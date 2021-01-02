

const contains = (arr, value = 0) => {
   const arrSize = arr.size();
   loop (let i=0, i < arrSize, i++) {
      if (arr[i] == value) return true;
   };
   return false;
};


const Set = (arr = []) => {
   const set = arr;

   set.push = [set](value) => {
      if (contains(set, value)) return;
      set.push(value);
   };

   set.has = [set](value) => {
      return contains(set, value);
   };

   return set;
};

let main = (a, b) => {
   const mySet = Set([1, 2, 3]);
   mySet.push(1);
   return mySet.has(2);
};