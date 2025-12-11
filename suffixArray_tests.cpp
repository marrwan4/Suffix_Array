#define SUFFIX_ARRAY_TEST
#include "suffixArray.cpp"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using namespace std;

static vector<int> buildExpected(const string &s)
{
    const int n = static_cast<int>(s.size());
    vector<int> idx(n);
    iota(idx.begin(), idx.end(), 0);
    auto cmp = [&](int a, int b) {
        const int lenA = n - a;
        const int lenB = n - b;
        const int cmpLen = min(lenA, lenB);
        int res = s.compare(a, cmpLen, s, b, cmpLen);
        if (res == 0)
        {
            return lenA < lenB;
        }
        return res < 0;
    };
    sort(idx.begin(), idx.end(), cmp);
    return idx;
}

static void printVector(const vector<int> &v)
{
    for (size_t i = 0; i < v.size(); i++)
    {
        cout << v[i];
        if (i + 1 < v.size())
        {
            cout << ' ';
        }
    }
    cout << endl;
}

static bool runCase(const string &label, const string &input)
{
    SuffixArray sa(input.c_str());
    sa.ConstructUsingPrefixDoubling();

    vector<int> got(sa.Data(), sa.Data() + sa.Size());
    const vector<int> expected = buildExpected(input);

    if (got != expected)
    {
        cout << "[FAIL] " << label << "\n  input: " << input << "\n  got:      ";
        printVector(got);
        cout << "  expected: ";
        printVector(expected);
        return false;
    }

    cout << "[PASS] " << label << endl;
    cout << "   input: " << input << "\n   suffix array: ";
    printVector(got);
    cout << "   expected: ";
    printVector(expected);
    return true;
}

int main()
{
    const vector<pair<string, string>> cases = {
        {"single char", "A"},
        {"two chars ascending", "AB"},
        {"two chars descending", "BA"},
        {"all same", "AAAA"},
        {"all same with dollar", "AAAA$"},
        {"banana", "BANANA$"},
        {"alternating", "ABAB"},
        {"palindrome with dollar", "ABABA$"},
        {"gattaca", "GATTACA$"},
        {"provided sample", "ACGACTACGATAAC$"},
        {"mississippi", "MISSISSIPPI$"},
        {"small mixed", "ZXY"},
        {"duplicate letters", "CABBA$"},
        {"phrase", "TOBEORNOTTOBE$"},
        {"all P", "PPPPP$"},
        {"alphabet", "ABCDEFG$"},
        {"descending run", "ZYXWV$"},
        {"repeated block", "ABCABCABC$"},
        {"many As", "AAAAAAAAAA$"},
        {"abcdabcd", "ABCDABCD$"},
        {"panama bananas", "PANAMABANANAS$"}
    };

    int passed = 0;
    for (const auto &c : cases)
    {
        if (runCase(c.first, c.second))
        {
            passed++;
        }
    }

    cout << "\nResult: " << passed << "/" << cases.size() << " test cases passed." << endl;
    return passed == static_cast<int>(cases.size()) ? 0 : 1;
}
