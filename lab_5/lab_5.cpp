#include <iostream>

const std::string hex_table = "0123456789ABCDEF";

std::string data_encode(std::string data, std::string key) {
    std::string result(data.length(), 0);

    for (int i = 0; i < data.length(); i++) {
        result[i] = data[i] ^ key[i % key.length()];
    }

    return result;
}

std::string bin_to_hex(std::string src) {
    std::string res(src.size() * 2, '0');

    for (int i = 0; i < src.length(); i++) {
        res[i * 2] = hex_table[src[i] / 16];
        res[i * 2 + 1] = hex_table[src[i] % 16];
    }

    return res;
}

int main()
{
    std::string data = "Shannon contributed to the field of cryptanalysis for national defense during World War II, \nincluding his basic work on codebreaking and secure telecommunications.\n";
    std::string key = "Shannon";
    std::cout << bin_to_hex(data_encode(data, key));
}
