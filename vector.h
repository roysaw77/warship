template <typename T>
class vector{
    private:
        size_t size;
        size_t capacity;
        T *data;

        void resize(){
            if(size == capacity){
                capacity *= 2;
                T *new_data = new T[capacity];
                for(int i = 0; i < size; i++){
                    new_data[i] = data[i];
                }
                delete[] data;
                data = new_data;
            }
        };

    public:
        vector(): data(nullptr),size(0),capacity(0){};
         ~vector(){
            delete[] data;
        };

        void push_back(const T& value){
            if(size==capacity){
                resize();
            }
            data[size++] = value;
        };
        T& operator[](size_t index) {
        return data[index]; 
        }

        const T& operator[](size_t index) const {
        return data[index]; 
       }

        size_t length() const {
        return size;
       }
         size_t get_capacity() const {
        return capacity;
       }

        void clear() {
        size = 0;
    }
};
