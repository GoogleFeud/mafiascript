

const Person = (name, color = "black", age) => {
   const obj = {
      "name": name,
      "color": color,
      "colour": color,
      "age": age
   };

   obj.isUnderage = [obj](minAge = 18) => {
      if (obj.age < minAge) return true;
      return false;
   };

   return obj;
};

const people = [];

loop (let i=0, i < 10000, i++) {
   people.push(Person("", null, i.copy()));
};

let main = (a, b) => {
   return people.map((p) => p.age);
};