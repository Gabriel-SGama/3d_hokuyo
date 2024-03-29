#include "headers/Hokuyo.hpp"

using namespace qrk;
using namespace std;

Hokuyo::Hokuyo(int argc, char *argv[]) {
    info = new Connection_information(argc, argv);

    cout << "trying to connect to hokuyo" << endl;

    // Connects to the sensor
    if (!urg.open(info->device_or_ip_name(),
                  info->baudrate_or_port_number(),
                  info->connection_type())) {
        cout << "Urg_driver::open(): "
             << info->device_or_ip_name() << ": " << urg.what() << endl;
        exit(1);
    }

    // config scan parameters
    first_step = urg.deg2step(-_VISION_ANGLE);
    last_step = urg.deg2step(_VISION_ANGLE);
    skip_step = 0;

    urg.set_scanning_parameter(first_step, last_step, skip_step);
    max_data_size = urg.max_data_size();
    dataTemp.resize(max_data_size);

    print_info();

    mode = HOKUYO_READER;
    urg.start_measurement(Urg_driver::Distance, Urg_driver::Infinity_times, 0);
}

Hokuyo::Hokuyo(string filename) {
    rfname = filename;
    cout << "Loading " << filename << endl;

    inFile.open(rfname.c_str(), ios::in | ios::binary);

    if (!inFile.is_open()) {
        cout << "could not open " << filename << endl;
        exit(1);
    }

    inFile.read((char *)&max_data_size, sizeof(int));

    cout << "max_data_size: " << max_data_size << endl;

    dataTemp.resize(max_data_size);

    mode = FILE_READER;
}

void Hokuyo::setWrite(string wfname, int max_data_size) {
    this->wfname = wfname;
    this->max_data_size = max_data_size;

    outFile.open(wfname.c_str(), ios::out | ios::binary);
    outFile.write(reinterpret_cast<char *>(&max_data_size), sizeof(int));
}

void Hokuyo::writeFile(Scan *data) {
    if (!outFile.is_open()) {
        cout << "set write file first" << endl;
        exit(1);
    }

    int menos_um = -1;

    for (int i = 0; i < data->size; i++) {
        outFile.write(reinterpret_cast<char *>(&data->pts[i].real_x), sizeof(int));
        outFile.write(reinterpret_cast<char *>(&data->pts[i].y), sizeof(int));
    }
    outFile.write(reinterpret_cast<char *>(&menos_um), sizeof(int));  // end of scan
}

int Hokuyo::read_file(Scan *data, long &timeStamp) {
    int real_x, x, y;
    int i = 0;

    while (!inFile.eof()) {
        inFile.read((char *)&real_x, sizeof(int));
        if (real_x == -1) {
            data->size = i;
            return 1;
        }
        inFile.read((char *)&y, sizeof(int));

        if (real_x == 0 && y == 0)
            continue;

        data->pts[i].real_x = real_x;
        x = real_x * angle_cos;
        data->pts[i].x = x;
        data->pts[i].y = y;
        data->pts[i].objID = 0;
        data->pts[i].dist = (int)sqrt(x * x + y * y);
        data->pts[i].angle = atan2(y, x);
        data->pts[i].x_img = round(-y * scaley + _WIDTH / 2.0);
        data->pts[i].y_img = round(-x * scalex + _HEIGHT / 2.0);

        // cout << "(" << x << "," << y << ")" << endl;
        i++;
    }
    // cout << "i: " << i << endl;
    return 0;
}

void Hokuyo::convertXY(Scan *data) {
    long min_distance = urg.min_distance();
    long max_distance = urg.max_distance();
    size_t data_n = dataTemp.size();

    cout << "data_n: " << data_n << endl;

    for (size_t i = 0; i < data_n; ++i) {
        long l = dataTemp[i];
        if ((l <= min_distance) || (l >= max_distance)) {
            continue;
        }

        double radian = urg.index2rad(i);
        data->pts[i].real_x = static_cast<int>(l * cos(radian));
        data->pts[i].x = data->pts[i].real_x * angle_cos;
        data->pts[i].y = static_cast<int>(l * sin(radian));

        data->pts[i].objID = 0;
        data->pts[i].dist = l;
        data->pts[i].angle = radian;
        data->pts[i].x_img = round(-data->pts[i].y * scaley + _WIDTH / 2.0);
        data->pts[i].y_img = round(-data->pts[i].x * scalex + _HEIGHT / 2.0);
    }

    cout << "data converted" << endl;

    data->size = data_n;
}

int Hokuyo::hokuyoReader(Scan *data, long &timeStamp) {
    cout << "reading hokuyo" << endl;
    if (!urg.get_distance(dataTemp, &timeStamp)) {
        cout << "Urg_driver::get_distance(): " << urg.what() << endl;

        if (urg.what() == "checksum error.") {
            cout << "reconnecting" << endl;
            if (!urg.open(info->device_or_ip_name(),
                          info->baudrate_or_port_number(),
                          info->connection_type())) {
                cout << "Urg_driver::open(): "
                     << info->device_or_ip_name() << ": " << urg.what() << endl;
                exit(1);
            }

            urg.set_scanning_parameter(first_step, last_step, skip_step);
            urg.start_measurement(Urg_driver::Distance, Urg_driver::Infinity_times, 0);
            return 1;
        }
        return 0;
    }

    cout << "convert to XY data" << endl;
    convertXY(data);

    return 1;
}

int Hokuyo::getData(Scan *data, long &timeStamp) {
    if (mode == HOKUYO_READER)
        return hokuyoReader(data, timeStamp);

    return read_file(data, timeStamp);
}

void Hokuyo::print_info() {
    // connection parameters
    cout << "hokuyo connected" << endl;
    cout << "device_or_ip_name: " << info->device_or_ip_name() << endl;
    cout << "baudrate_or_port_number: " << info->baudrate_or_port_number() << endl;
    cout << "connection_type: " << info->connection_type() << endl;

    // Displays sensor parameters
    cout << "Sensor product type: " << urg.product_type() << endl;
    cout << "Sensor firmware version: " << urg.firmware_version() << endl;
    cout << "Sensor serial ID: " << urg.serial_id() << endl;
    cout << "Sensor status: " << urg.status() << endl;
    cout << "Sensor state: " << urg.state() << endl;

    cout << "step: ["
         << urg.min_step() << ", "
         << urg.max_step() << "]" << endl;
    cout << "distance: ["
         << urg.min_distance()
         << ", " << urg.max_distance() << endl;

    cout << "scan interval: " << urg.scan_usec() << " [usec]" << endl;

    cout << "sensor data size: " << max_data_size << endl;
}

Hokuyo::~Hokuyo() {
    if (inFile.is_open()) inFile.close();
    if (outFile.is_open()) outFile.close();
    delete info;
}
