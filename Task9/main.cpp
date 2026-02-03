#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <iomanip>
#include <sstream>
#include <cassert>
#include <limits>
using namespace std;

class BigNatural {
protected:
    vector<int> digits;
    static const int BASE = 1000000000;

    void trim() {
        while (digits.size() > 1 && digits.back() == 0) {
            digits.pop_back();
        }
    }

public:
    BigNatural() {
        digits.push_back(0);
    }

    BigNatural(unsigned long long n) {
        if (n == 0) digits.push_back(0);
        while (n > 0) {
            digits.push_back(n % BASE);
            n /= BASE;
        }
    }

    BigNatural(string s) {
        if (s.empty()) {
            digits.push_back(0);
        } else {
            for (int i = (int)s.length(); i > 0; i -= 9) {
                if (i < 9)
                    digits.push_back(stoi(s.substr(0, i)));
                else
                    digits.push_back(stoi(s.substr(i - 9, 9)));
            }
        }
        trim();
    }

    string toString() const {
        if (digits.empty()) return "0";
        string s = to_string(digits.back());
        for (int i = (int)digits.size() - 2; i >= 0; --i) {
            string tmp = to_string(digits[i]);
            while (tmp.length() < 9) tmp = "0" + tmp;
            s += tmp;
        }
        return s;
    }

    long long toLongLong() const {
        long long res = 0;
        long long multiplier = 1;
        if (digits.size() > 3) throw out_of_range("BigNatural too large for long long");

        for (size_t i = 0; i < digits.size(); ++i) {
            res += digits[i] * multiplier;
            multiplier *= BASE;
        }
        return res;
    }

    bool operator==(const BigNatural& other) const {
        return digits == other.digits;
    }

    bool operator!=(const BigNatural& other) const {
        return !(*this == other);
    }

    bool operator<(const BigNatural& other) const {
        if (digits.size() != other.digits.size())
            return digits.size() < other.digits.size();
        for (int i = digits.size() - 1; i >= 0; --i) {
            if (digits[i] != other.digits[i])
                return digits[i] < other.digits[i];
        }
        return false;
    }

    bool operator>(const BigNatural& other) const { return other < *this; }
    bool operator<=(const BigNatural& other) const { return !(*this > other); }
    bool operator>=(const BigNatural& other) const { return !(*this < other); }

    BigNatural operator+(const BigNatural& other) const {
        BigNatural res;
        res.digits.clear();
        int carry = 0;
        size_t n = max(digits.size(), other.digits.size());

        for (size_t i = 0; i < n || carry; ++i) {
            long long sum = carry + (long long)(i < digits.size() ? digits[i] : 0)
                                  + (long long)(i < other.digits.size() ? other.digits[i] : 0);
            res.digits.push_back(sum % BASE);
            carry = sum / BASE;
        }
        return res;
    }

    BigNatural operator-(const BigNatural& other) const {
        if (*this < other) throw invalid_argument("Result of subtraction is negative (in BigNatural)");
        BigNatural res;
        res.digits.clear();
        int borrow = 0;
        for (size_t i = 0; i < digits.size(); ++i) {
            long long sub = (long long)digits[i] - borrow - (i < other.digits.size() ? other.digits[i] : 0);
            if (sub < 0) {
                sub += BASE;
                borrow = 1;
            } else {
                borrow = 0;
            }
            res.digits.push_back(sub);
        }
        res.trim();
        return res;
    }

    BigNatural operator*(const BigNatural& other) const {
        BigNatural res;
        res.digits.resize(digits.size() + other.digits.size(), 0);

        for (size_t i = 0; i < digits.size(); ++i) {
            long long carry = 0;
            for (size_t j = 0; j < other.digits.size() || carry; ++j) {
                long long cur = res.digits[i + j] + digits[i] * 1LL * (j < other.digits.size() ? other.digits[j] : 0) + carry;
                res.digits[i + j] = cur % BASE;
                carry = cur / BASE;
            }
        }
        res.trim();
        return res;
    }

    static pair<BigNatural, BigNatural> div_mod(const BigNatural& a, const BigNatural& b) {
        if (b.digits.size() == 1 && b.digits[0] == 0) throw runtime_error("Division by zero");
        if (a < b) return {BigNatural(0), a};

        BigNatural quotient;
        quotient.digits.resize(a.digits.size());
        BigNatural remainder = 0;

        for (int i = a.digits.size() - 1; i >= 0; --i) {
            if (!(remainder.digits.size() == 1 && remainder.digits[0] == 0)) {
                remainder.digits.insert(remainder.digits.begin(), a.digits[i]);
            } else {
                remainder.digits[0] = a.digits[i];
            }

            int l = 0, r = BASE - 1;
            int best = 0;
            while (l <= r) {
                int m = l + (r - l) / 2;
                if (b * BigNatural(m) <= remainder) {
                    best = m;
                    l = m + 1;
                } else {
                    r = m - 1;
                }
            }

            quotient.digits[i] = best;
            remainder = remainder - (b * BigNatural(best));
        }
        quotient.trim();
        remainder.trim();
        return {quotient, remainder};
    }

    BigNatural operator/(const BigNatural& other) const {

        return div_mod(*this, other).first;
    }

    BigNatural operator%(const BigNatural& other) const {
        return div_mod(*this, other).second;
    }

    bool isZero() const {
        return digits.size() == 1 && digits[0] == 0;
    }
};

class BigInt {
private:
    BigNatural value;
    bool is_negative;

public:
    BigInt() : value(0), is_negative(false) {}

    BigInt(long long n) {
        if (n < 0) {
            is_negative = true;
            if (n == std::numeric_limits<long long>::min()) {
                 value = BigNatural(to_string(n).substr(1));
            } else {
                value = BigNatural((unsigned long long)(-n));
            }
        } else {
            is_negative = false;
            value = BigNatural((unsigned long long)n);
        }
    }

    BigInt(string s) {
        if (s.empty()) { value = 0; is_negative = false; return; }
        if (s[0] == '-') {
            is_negative = true;
            value = BigNatural(s.substr(1));
        } else {
            is_negative = false;
            value = BigNatural(s);
        }
        if (value.isZero()) is_negative = false;
    }

    BigInt(BigNatural val, bool neg) : value(val), is_negative(neg) {
        if (val.isZero()) is_negative = false;
    }

    string toString() const {
        string s = value.toString();
        if (is_negative && s != "0") return "-" + s;
        return s;
    }

    long long toLongLong() const {
        long long res = value.toLongLong();
        if (is_negative) return -res;
        return res;
    }

    bool operator==(const BigInt& other) const {
        return is_negative == other.is_negative && value == other.value;
    }
    bool operator!=(const BigInt& other) const { return !(*this == other); }

    bool operator<(const BigInt& other) const {
        if (is_negative != other.is_negative) return is_negative;
        if (is_negative) return value > other.value;
        return value < other.value;
    }
    bool operator>(const BigInt& other) const { return other < *this; }
    bool operator<=(const BigInt& other) const { return !(*this > other); }
    bool operator>=(const BigInt& other) const { return !(*this < other); }

    BigInt operator-() const {
        if (value.isZero()) return *this;
        return BigInt(value, !is_negative);
    }

    BigInt operator+(const BigInt& other) const {
        if (is_negative == other.is_negative) {
            return BigInt(value + other.value, is_negative);
        }
        if (value >= other.value) {
            return BigInt(value - other.value, is_negative);
        } else {
            return BigInt(other.value - value, other.is_negative);
        }
    }

    BigInt operator-(const BigInt& other) const {
        return *this + (-other);
    }

    BigInt operator*(const BigInt& other) const {
        return BigInt(value * other.value, is_negative != other.is_negative);
    }

    BigInt operator/(const BigInt& other) const {
        if (other.value.isZero()) throw runtime_error("Division by zero");
        return BigInt(value / other.value, is_negative != other.is_negative);
    }

    BigInt operator%(const BigInt& other) const {
        return BigInt(value % other.value, is_negative);
    }

    static BigInt pow(BigInt base, BigInt exp) {
        if (exp < BigInt(0)) throw runtime_error("Negative exponent not supported");
        BigInt res(1);
        while (exp > BigInt(0)) {
            if (exp % BigInt(2) != BigInt(0)) res = res * base;
            base = base * base;
            exp = exp / BigInt(2);
        }
        return res;
    }

    friend ostream& operator<<(ostream& os, const BigInt& bi) {
        os << bi.toString();
        return os;
    }
};

void runTests() {
    cout << "Running extended tests..." << endl;

    // --- 1. Basic Tests ---
    assert((BigInt("100") + BigInt("200")).toString() == "300");
    assert((BigInt("-100") + BigInt("-200")).toString() == "-300");
    assert((BigInt("100") - BigInt("-50")).toString() == "150");
    assert((BigInt("50") - BigInt("100")).toString() == "-50");
    cout << "[OK] Basic arithmetic tests passed." << endl;

    // --- 2. Long number tests ---
    string s_zeros_100(100, '0');
    string s_nines_100(100, '9');
    BigInt b_10_100("1" + s_zeros_100);
    BigInt b_nines(s_nines_100);

    assert((b_nines + BigInt(1)).toString() == b_10_100.toString());
    assert((b_10_100 - BigInt(1)).toString() == b_nines.toString());
    cout << "[OK] Long number arithmetic passed." << endl;

    // --- 3. Edge Cases: Division by Zero ---
    try {
        BigInt a("100");
        BigInt b("0");
        BigInt c = a / b;
        assert(false && "Division by zero did not throw exception!");
    } catch (const runtime_error& e) {
        assert(string(e.what()) == "Division by zero");
    }
    cout << "[OK] Division by zero exception test passed." << endl;

    // --- 4. Edge Cases: Power 0^0 ---
    assert(BigInt::pow(BigInt(0), BigInt(0)).toString() == "1");
    cout << "[OK] 0^0 test passed." << endl;

    // --- 5. Advanced Comparisons ---
    BigInt big1("1" + string(100, '0'));       // 10^100
    BigInt big2("1" + string(100, '0'));       // 10^100 (copy)
    BigInt big3("1" + string(99, '0') + "1");  // 10^100 + 1

    BigInt bigSmall(string(100, '9'));

    assert(big1 == big2);
    assert(big1 != bigSmall);
    assert(big1 > bigSmall);
    assert(bigSmall < big1);
    assert(big1 >= big2);
    assert(big1 <= big2);

    BigInt negBig1 = -big1;
    BigInt negBigSmall = -bigSmall;

    assert(negBig1 < negBigSmall);
    assert(negBigSmall > negBig1);

    cout << "[OK] Advanced comparison tests passed." << endl;

    // --- 6. Previous Tests (Mult, Div, Mod) ---
    string s_zeros_50(50, '0');
    BigInt b_10_50("1" + s_zeros_50);
    assert((b_10_50 * b_10_50).toString() == b_10_100.toString());
    assert((b_10_100 / b_10_50).toString() == b_10_50.toString());

    assert((BigInt("10") % BigInt("3")).toString() == "1");
    assert((BigInt("-10") % BigInt("3")).toString() == "-1");

    cout << "ALL TESTS PASSED SUCCESSFULLY" << endl;
}


int main() {
    try {
        runTests();
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    BigInt a("12345678901234567890");
    BigInt b("100");

    BigInt sum = a + b;
    BigInt product = a * b;
    BigInt div = a / b;
    BigInt mod = a % b;

    BigInt power = BigInt::pow(a, b);

    std::cout << "Sum: " << sum << std::endl;
    std::cout << "a^b: " << power << std::endl; // very long number
    return 0;
}