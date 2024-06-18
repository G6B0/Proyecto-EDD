#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

class SuffixArray {
public:
    SuffixArray(const std::string &text) : text(text) {
        buildSuffixArray();
        buildLCPArray();
    }

    std::pair<int, int> longestMatch(const std::string &pattern, int start) const {
        int m = pattern.size();
        int n = text.size();

        int l = 0, r = n;
        while (l < r) {
            int mid = (l + r) / 2;
            if (text.compare(suffixArray[mid], m, pattern) < 0)
                l = mid + 1;
            else
                r = mid;
        }

        int bestLength = 0;
        int bestPosition = -1;

        for (int i = l; i < n && text.compare(suffixArray[i], m, pattern) == 0; ++i) {
            int length = 0;
            while (length < m && suffixArray[i] + length < n && pattern[length] == text[suffixArray[i] + length])
                ++length;

            if (length > bestLength && suffixArray[i] < start) {
                bestLength = length;
                bestPosition = suffixArray[i];
            }
        }

        return {bestPosition, bestLength};
    }

private:
    std::string text;
    std::vector<int> suffixArray;
    std::vector<int> lcpArray;

    void buildSuffixArray() {
        int n = text.size();
        suffixArray.resize(n);
        std::vector<int> rank(n), temp(n);

        for (int i = 0; i < n; ++i) {
            suffixArray[i] = i;
            rank[i] = text[i];
        }

        for (int k = 1; k < n; k *= 2) {
            auto cmp = [&](int a, int b) {
                if (rank[a] != rank[b])
                    return rank[a] < rank[b];
                int ra = a + k < n ? rank[a + k] : -1;
                int rb = b + k < n ? rank[b + k] : -1;
                return ra < rb;
            };

            std::sort(suffixArray.begin(), suffixArray.end(), cmp);

            temp[suffixArray[0]] = 0;
            for (int i = 1; i < n; ++i) {
                temp[suffixArray[i]] = temp[suffixArray[i - 1]] + cmp(suffixArray[i - 1], suffixArray[i]);
            }

            rank = temp;
        }
    }

    void buildLCPArray() {
        int n = text.size();
        lcpArray.resize(n);
        std::vector<int> rank(n);
        for (int i = 0; i < n; ++i) rank[suffixArray[i]] = i;

        int h = 0;
        for (int i = 0; i < n; ++i) {
            if (rank[i] > 0) {
                int j = suffixArray[rank[i] - 1];
                while (i + h < n && j + h < n && text[i + h] == text[j + h]) ++h;
                lcpArray[rank[i]] = h;
                if (h > 0) --h;
            }
        }
    }
};
