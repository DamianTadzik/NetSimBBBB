@startuml
' TODO

' --------

' ==== < CONFIGURATION > ====

hide empty members
hide <<function>> circle

' ==== END of < CONFIGURATION > ====

package "Storage" {

	interface IPackageStockpile <<interface>> {
	    + {abstract} push(Package&&) : void
	    + {abstract} empty() : bool {query}
	    + {abstract} size() : size_type {query}
	    + {method} /iteratory/
	    + ~IPackageStockpile()
	}

	interface IPackageQueue <<interface>> {
	    + {abstract} pop() : Package
	    + {abstract} get_queue_type() : PackageQueueType {query}
	}
	IPackageQueue .. IPackageStockpile

	enum PackageQueueType <<enumeration>> {
		FIFO
		LIFO
	}
	PackageQueueType .. IPackageQueue

	class PackageQueue {
	    + PackageQueue(PackageQueueType)
	}
	PackageQueue .. IPackageQueue

	class Package {
		+ Package()
		+ Package(ElementID)
		+ Package(Package&&)
		+ operator=(Package&&) : Package&
		+ get_id(): ElementID {query}
		+ ~Package()
	}

}

' == END of package "Storage"

' .. DATA TYPES --
together {
	class ElementID << (T,orchid) primitive>>
}

' == END of together


' ==== END of < POSITIONING > ====
@enduml