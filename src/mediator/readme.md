## Mediator Design Pattern

### Benefits of Mediator Design Pattern

1. You can replace any component in the system without affecting other
component & system.
2. Mediator Design Pattern reduces the complexity of communication between
the different components in a system. Thus promoting loose coupling & less
number of subclasses.
3. As to overcome the limittion of the Observer Design Pattern which works
in a one-to-many relationship, Mediator Design Pattern can be employed for
a many-to-many relationship.

### Summary by FAQs

* **Mediator vs Facade Design Pattern?**

    Mediator Pattern can be seen as a multiplexed facade pattern. In mediator,
    instead of working with an interface of a single object, you are making a
    multiplexed interface among multiple objects to provide smooth transitions.

* **Mediator vs Observer Design Pattern?**

    Observer Design Pattern = one-to-many relationship.

    Meditor Design Pattern = many-to-many relationship.

    Due to centralized control of communication, maintenance of the system
    designed using Mediator Design Pattern is easy.

* **Senders & Receivers Patterns**

    Chain of Responsibility, COmmand, Mediator, and Observer, address how
    you cna decouple senders and receivers, but with different tradeoffs.
    Chain of Responsibility passes a sender request along a chain of potential
    receivers. Command normally specifies a sender-receiver connection with a
    subclass. Mediator has senders and receivers reference each other
    indirectly. Observer defines a very decoupled interface that allows for
    multiple receivers to be configured at runtime.



