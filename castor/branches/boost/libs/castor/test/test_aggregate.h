struct integer {
    int i;

    integer(int i) : i(i) {}

    bool operator==(const integer& rhs) const {
        return i == rhs.i;
    }

    integer operator/ (const integer& rhs) const {
        return i / rhs.i;
    }
};

struct integer_add {
    integer operator()(const integer& lhs, const integer& rhs) const {
        return integer(lhs.i + rhs.i);
    }
};

struct integer_mult {
    integer operator()(const integer& lhs, const integer& rhs) const {
        return integer(lhs.i * rhs.i);
    }
};

bool isEven(int i) {
    return i % 2 == 0;
}
