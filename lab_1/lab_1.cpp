#include <iostream>
#include <vector>

using binary = std::vector<uint8_t>;

const std::string base64_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string bin_to_base64(binary src) {
    int res_buf_size = (src.size() + 2) / 3 * 4;
    std::string res(res_buf_size, 0);

    int block_count = src.size() / 3;
    for (int i = 0; i < block_count; i++) {
        uint32_t buf = (src[i * 3] << 16) | (src[i * 3 + 1] << 8) | (src[i * 3 + 2]);

        uint8_t c1_ind = (buf >> 18) & 0b00111111;
        uint8_t c2_ind = (buf >> 12) & 0b00111111;
        uint8_t c3_ind = (buf >> 6) & 0b00111111;
        uint8_t c4_ind = (buf >> 0) & 0b00111111;

        res[i * 4 + 0] = base64_table[c1_ind];
        res[i * 4 + 1] = base64_table[c2_ind];
        res[i * 4 + 2] = base64_table[c3_ind];
        res[i * 4 + 3] = base64_table[c4_ind];
    }

    int end_count = src.size() % 3;
    if (end_count > 0) {
        if (end_count == 1) {
            uint32_t buf = (src[block_count * 3] << 16);
            uint8_t c1_ind = (buf >> 18) & 0b00111111;
            uint8_t c2_ind = (buf >> 12) & 0b00111111;

            res[block_count * 4 + 0] = base64_table[c1_ind];
            res[block_count * 4 + 1] = base64_table[c2_ind];
            res[block_count * 4 + 2] = '=';
        }
        else {
            uint32_t buf = (src[block_count * 3] << 16) | (src[block_count * 3 + 1] << 8);
            uint8_t c1_ind = (buf >> 18) & 0b00111111;
            uint8_t c2_ind = (buf >> 12) & 0b00111111;
            uint8_t c3_ind = (buf >> 6) & 0b00111111;

            res[block_count * 4 + 0] = base64_table[c1_ind];
            res[block_count * 4 + 1] = base64_table[c2_ind];
            res[block_count * 4 + 2] = base64_table[c3_ind];
        }

        res[block_count * 4 + 3] = '=';
    }

    return res;
}

int char_to_int(char c) {
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
}

binary hex_to_bin(std::string src) {
    binary res(src.length() / 2, 0);

    for (int i = 0; i < res.size(); i++) {
        res[i] = char_to_int(src[i * 2]) * 16 + char_to_int(src[i * 2 + 1]);
    }

    return res;
}

std::string hex_to_base64(std::string src) {
    return bin_to_base64(hex_to_bin(src));
}

int main() {
    std::string src = "49276d207374756479696e672043727970746f677261706879206c696b6520436c6175646520456c776f6f64205368616e6e6f6e21";
    //std::cin >> src;
    std::cout << hex_to_base64(src) << std::endl;
}
