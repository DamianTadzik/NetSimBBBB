@startuml
' TODO

' --------

' ==== < CONFIGURATION > ====

hide empty members
hide <<function>> circle

' ==== END of < CONFIGURATION > ====

package "Factory Package" {

	class NodeCollection<Node> << (T,olive) >> {
	    + add(node: Node&&) : void
	    + remove_by_id(id: ElementID) : void
	    + find_by_id(id: ElementID) : NodeCollection<Node>::iterator
	    + find_by_id(id: ElementID) : NodeCollection<Node>::const_iterator {query}
	    + {method} /iteratory/
	}

	Ramps .. NodeCollection
	Workers .. NodeCollection
	Storehouses .. NodeCollection

	class Factory {
		+ add_ramp(Ramp&&) : void
		+ remove_ramp(id: ElementID) : void
		+ find_ramp_by_id(id: ElementID) : NodeCollection<Ramp>::iterator
		+ find_ramp_by_id(id: ElementID) : NodeCollection<Ramp>::const_iterator {query}
	    + ramp_cbegin() : NodeCollection<Ramp>::const_iterator {query}
	    + ramp_cend() : NodeCollection<Ramp>::const_iterator {query}

    	... (analogicznie dla pozostałych typów węzłów)

		- remove_receiver(collection: NodeCollection<Node>&, id: ElementID) : void

		+ is_consistent(void) : bool {query}
		+ do_deliveries(Time) : void
		+ do_package_passing(void) : void
		+ do_work(Time) : void
	}
	Factory .. Ramps
	Factory .. Workers
	Factory .. Storehouses

}

' == END of package "Factory"


' ==== END of < POSITIONING > ====
@enduml