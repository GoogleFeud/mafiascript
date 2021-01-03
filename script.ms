

const contains = (arr, value = 0) => {
   const arrSize = arr.size();
   loop (let i=0, i < arrSize, i++) {
      if (arr[i] == value) return true;
   };
   return false;
};


const Queue = () => {
   const q = _arguments;   

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

const weirdFn = (firstParam) => {
   const restParams = _arguments;
   restParams.delete(0);
   return "First param is " + firstParam + " the rest are: " + restParams.map((p) => p).join(", ");
};

const main = (a, b) => {
   const myQ = Queue(1, 2, 3, 4, 5, 6);
   return weirdFn(1, 2, 3, 4, 5, 6, 7, 8, 9);
};