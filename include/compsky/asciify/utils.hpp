template<typename T>
size_t count_digits(T n){
    size_t n_digits = 0;
    do {
        ++n_digits;
        n /= 10;
    } while (n != 0); // Note that "0" has 1 digit, so a while{} loop cannot be used.
    return n_digits;
};
