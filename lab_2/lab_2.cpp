#include <iostream>
#include <vector>

using binary = std::vector<uint8_t>;

const std::string hex_table = "0123456789ABCDEF";

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

std::string bin_to_hex(binary src) {
    std::string res(src.size() * 2, '0');

    for (int i = 0; i < src.size(); i++) {
        res[i * 2] = hex_table[src[i] / 16];
        res[i * 2 + 1] = hex_table[src[i] % 16];
    }

    return res;
}

binary bin_xor(binary bin_1, binary bin_2) {
    if (bin_1.size() != bin_2.size()) {
        return binary();
    }

    binary res(bin_1.size(), 0);

    for (int i = 0; i < res.size(); i++) {
        res[i] = bin_1[i] ^ bin_2[i];
    }

    return res;
}

int main() {
    std::string s1 = "506561636520416c6c204f7665722054686520576f726c64", s2 = "4949544353551c0111001f010100061a021f010100061a02";
    //std::cin >> s1 >> s2;
    std::cout << bin_to_hex(bin_xor(hex_to_bin(s1), hex_to_bin(s2)));
}
