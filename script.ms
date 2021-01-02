

const contains = (arr, value = 0) => {
   const arrSize = arr.size();
   loop (let i=0, i < arrSize, i++) {
      if (arr[i] == value) return true;
   };
   return false;
};


const Queue = (els = []) => {
   const q = els;

   q.enqueue = [q](val) => {
      q.insert(val);
      return q;
   };

   q.dequeue = [q]() => {
      return q.pop();
   };

   return q;
};

const Map = (entries = []) => {
   const map = {};
   let _inner = {};
   let _size = entries.size();

   entries.each([_inner](el) => {
      _inner[el[0]] = el[1];
   });

   map.set = [_inner, _size](key, val) => {
      _inner[key] = val;
      _size++;
   };

   map.get = [_inner](key) => {
      return _inner[key];
   };

   map.delete = [_inner, _size](key) => {
      _inner[key] = null;
      _size--;
   };

   map.has = [_inner](key) => {
      return ((typeof _inner[key]) != "null");
   };

   map.clear = [_inner, _size]() => {
      _inner = {};
      _size = 0;
   };

   map.size = [_size]() => {
      return _size;
   };

   map.toString = [_size]() => {
      return "Map [" + _size + "]";
   };

   return map;
};

let main = (a, b) => {
   const arr = [];
   arr.insert = [arr](val) => {
      print(val + arr.size());
   };

   arr.push(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);

   const arr2 = arr;

   const insert = arr2.insert;

   const arr3 = [1, 2, 3];
   arr3.insert = arr.insert;

   const insert2 = arr3.insert;

   insert(10);
   insert2(1);

   return 1;
};