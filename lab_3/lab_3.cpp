#include <iostream>
#include <vector>
#include <optional>
#include <algorithm>
#include <map>
#include <cmath>

using binary = std::vector<uint8_t>;

std::map<char, float> occurance_english {
    {'a', 8.2389258}, {'b', 1.5051398}, {'c', 2.8065007}, {'d', 4.2904556},
    {'e', 12.813865}, {'f', 2.2476217}, {'g', 2.0327458}, {'h', 6.1476691},
    {'i', 6.1476691}, {'j', 0.1543474}, {'k', 0.7787989}, {'l', 4.0604477},
    {'m', 2.4271893}, {'n', 6.8084376}, {'o', 7.5731132}, {'p', 1.9459884},
    {'q', 0.0958366}, {'r', 6.0397268}, {'s', 6.3827211}, {'t', 9.1357551},
    {'u', 2.7822893}, {'v', 0.9866131}, {'w', 2.3807842}, {'x', 0.1513210},
    {'y', 1.9913847}, {'z', 0.0746517}
};
    
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

binary bin_xor(binary bin, uint8_t key) {
    binary res(bin.size(), 0);
    for (int i = 0; i < res.size(); i++)
        res[i] = bin[i] ^ key;
    return res;
}

float test(binary src) {
    std::map<char, float> occurance_text;

    int char_count = 0;
    for (auto chr : src) {
        if (chr >= 'a' && chr <= 'z') {
            occurance_text[chr]++;
            char_count++;
        }
    }

    for (auto& chr : occurance_text) {
        chr.second = chr.second/src.size();
    }

    float sum = 0.0;
    for (auto chr : occurance_english) {
        sum += pow(chr.second - occurance_text[chr.first], 2);
    }

    return sum/char_count;
}

std::optional<std::string> decipher(binary src) {
    std::string res(src.size() * 2, '0');

    for (int i = 0; i < src.size() - 1; i++) {
        uint8_t b1 = src[i] & 0x80;
        uint8_t b2 = src[i + 1] & 0x80;

        if (b1 != b2) {
            return std::nullopt;
        }
    }

    uint8_t key = 0x0;
    if ((src[0] & 0x80) == 0x80) {
        key = 0x80;
    }

    int b00 = 0, b01 = 0, b10 = 0, b11 = 0;
    for (int i = 0; i < src.size(); i++) {
        switch (src[i] & 0x60) {
            case 0x00: b00++; break;
            case 0x20: b01++; break;
            case 0x40: b10++; break;
            case 0x60: b11++; break;
        }
    }

    if (b00 == std::max({b00, b01, b10, b11})) {
        key ^= 0x60;
    } else if (b01 == std::max({ b00, b01, b10, b11 })) {
        key ^= 0x40;
    } else if (b10 == std::max({ b00, b01, b10, b11 })) {
        key ^= 0x20;
    }

    std::vector<std::pair<float, binary>> variants;
    for (int key_offset = 0; key_offset < 32; key_offset++) {
        binary str_temp = bin_xor(src, key + key_offset);
        variants.push_back({ test(str_temp), str_temp });
    }

    std::string result;

    float min_variant = (*std::min_element(variants.begin(), variants.end())).first;
    for (int i = 0; i < 32; i++) {
        if (variants[i].first == min_variant) {
            key += i;
            result = std::string(variants[i].second.begin(), variants[i].second.end());
            break;
        }
    }

    /*
    std::cout << std::endl;
    std::cout << "Key: 0x" << std::hex << (unsigned int)key << std::dec << std::endl;
    std::cout << "b00: " << b00 << std::endl;
    std::cout << "b01: " << b01 << std::endl;
    std::cout << "b10: " << b10 << std::endl;
    std::cout << "b11: " << b11 << std::endl;
    */

    return { result };
}

int main()
{
    std::string str = "19367831362e3d2b2c353d362c783136783336372f343d3c3f3d7839342f39212b782839212b782c303d783a3d2b2c7831362c3d2a3d2b2c";
    std::cout << decipher(hex_to_bin(str)).value() << std::endl;
}
