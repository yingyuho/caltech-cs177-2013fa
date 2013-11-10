template<typename T>
class CyclicArray {
private:
   T* array;
public:
   const int size;

   CyclicArray(int size) : size(size) { array = new T[size]; }
   ~CyclicArray() { delete [] array; }

   T& operator[] (int i) { return array[(i<0)?size-(-i)%size:(i%size)]; }
   const T& operator[] (int i) const { return array[(i<0)?size-(-i)%size:(i%size)]; }
};
