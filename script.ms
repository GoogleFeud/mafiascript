const merge = (arr1 = [], arr2 = []) => {
   const new = [];
   loop(arr1.size() && arr2.size()) {
      if (arr1[0] < arr2[0]) {
         new.push(arr1[0]);
         arr1.delete(0);
      } else {
         new.push(arr2[0]);
         arr2.delete(0);
      };
   };
   new.merge(arr1, arr2);
   return new;
};

const mergeSort = (arr = []) => {
   if (arr.size() < 2) return arr;
   const middle = (arr.size() / 2).floor();
   const arr1 = mergeSort(arr.slice(0, middle));
   const arr2 = mergeSort(arr.slice(middle, arr.size()));
   return merge(arr1, arr2);
};

const binarySearch = (arr, value, start = 0, end = arr.size()) => {
   if (start > end) return false;
   const mid = ((start + end) / 2).floor();
   if (arr[mid] == value) return true;
   
   if (arr[mid] > value) return binarySearch(arr, value, start, mid - 1)
   else return binarySearch(arr, value, mid + 1, end);
};

const Person = (name, age) => {
   const _ = {
      "name": name,
      "age": age
   };

   _.isUnderage = |_|(minAge = 18) => {
      if (_.age > minAge) return false;
      return true;
   };

   return _;
};

const main = () => {
const me = Person("Google", 18);
print(me.isUnderage());
};


main();