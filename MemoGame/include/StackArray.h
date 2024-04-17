#ifndef STACKARRAY_H
#define STACKARRAY_H


namespace mz {
    template<int SIZE, typename TYPE>
    class StackArray {
        private:
            TYPE items[SIZE];

        public:
            template<typename... Args>
            StackArray(Args... args) : items{args...} {}

            StackArray(TYPE items[SIZE]) {
                for (int i = 0; i < SIZE; i++) {
                    this->items[i] = items[i];
                }
            }

            TYPE& operator[](unsigned int index) {
                return this->items[index];
            }

            unsigned int size() const;
    };
} // namespace mz

 
#endif