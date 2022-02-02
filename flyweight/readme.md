## Benefits of Flyweight Design Pattern

1. Facilitates the reuse of many fine-grained object, making the utilization of larg
numbers of objects more efficient.
2. Improves data caching for higher response time.
3. Data caching intern increases performance due to a lesser number of heavy objects.
4. Provide a centralized mechanism to control the states/common-attributes objects.

## Summary by FAQs

* **When to use a Flyweight Design Pattern?**

In need of a large number of objects.

When there is a prepetitive creation of heavy objects which can be replaced by a
few shared objects.

* **Difference between Singleton and Flyweight Design Pattern?**

In Singleton Design Pattern, you cannot create more than one object. You need to 
reuse the existing object in all parts of the application.

While in Flyweight Design Pattern you can have a large number of similar objects
which can share a common single resource.

* **Drawbacks of Flyweight Design Pattern?**

As similar to Singleton Design Pattern, concurrency is also a headache in the 
Flyweight Design Pattern. Without appropriate measures, if you create Flyweight
objects in a concurrent environment, you may end up having multiple instances of the
smae object which is not desirable.


