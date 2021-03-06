//Sample using an interface and derived class to demonstrate a few best practices for OOP in C++
//More samples available at https://github.com/jsparks125/Samples

#include <cstdio>
#include <iostream>
#include <memory>
#include <vector>

//no "using namespace std"

namespace samples
{
	//an object that might be too large to pass and return by value
	class LargeObject
	{
	public:
		//default constructor trivial and omitted
		LargeObject(std::vector<int> a_bunch_of_ints) : a_bunch_of_ints_(a_bunch_of_ints) {}
		//destructor trivial and omitted

		//inlined accessors and mutators with const correctness
		//I prefer to use lowercase underscore syntax even for atypical accessors and mutators
		inline int int_at(const int index) const
		{
			try 
			{ 
				return a_bunch_of_ints_.at(index); 
			}
			catch (std::exception e)
			{
				std::cerr << e.what() << "\n";
				return -1;
			}
		}
		//for simplicity, the members of this object's vector are not modifiable, so there are no mutators

	protected: 
		//nothing defined, kept to show proper ordering

	private:
		//just a bunch of ints (maybe)
		std::vector<int> a_bunch_of_ints_;
	};

	//interface demonstrating a composition relationship and the Template Method design pattern
	class BaseObject
	{
	public:
		//constructors
		BaseObject() { std::cout << "BaseObject default constructed" << std::endl; }
		//explicit constructor using initialization list
		BaseObject(const int base_int) : base_int_(base_int) 
		{ 
			std::cout << "BaseObject constructed with base_int_ = " << base_int << std::endl; 
		}
		//virtual destructor to properly destroy the object if deleting a pointer to a derived object
		virtual ~BaseObject() 
		{ 
			std::cout << "BaseObject destructed" << std::endl; 
		}

		//copy constructor and assignment operator trivial and left undeclared

		//inlined accessors and mutators with const correctness
		inline int base_int() const { return base_int_; }
		inline void set_base_int(const int base_int) { base_int_ = base_int; }

		//public facing interface function that calls virtual functions implemented by a derived class
		//this is declared const since both function calls are also const
		inline void VirtualInterfaceFunction() const
		{ 
			VirtualFunctionOne();
			VirtualFunctionTwo();
		}
		void PassByRefFunction(const LargeObject&);
		void PassByPtrFunction(const std::shared_ptr<LargeObject>);
		inline void PassByValue(const int x) 
		{ 
			std::cout << "x is small enough to be passed by value" << std::endl; 
		}

	protected:

	private:
		// maintaining a composition relationship by making this private
		int base_int_;

		//virtual const functions
		virtual void VirtualFunctionOne() const = 0;
		virtual void VirtualFunctionTwo() const = 0;
	};
	
	void BaseObject::PassByRefFunction(const LargeObject& derived_object)
	{
		std::cout << "This long function's argument cannot be modified and cannot be null" << std::endl;
	}

	void BaseObject::PassByPtrFunction(const std::shared_ptr<LargeObject> derived_object)
	{
		std::cout << "This long function's argument cannot be modified and can be null" << std::endl;
	}

	//implementation of the above interface
	class DerivedObject : public BaseObject
	{
	public:
		//unncessary default constructor omitted
		//constructor calling specific constructor for BaseObject and using initialization list with base class first
		DerivedObject(int derived_int, int base_int) : BaseObject(base_int), derived_int_(derived_int) 
		{
			std::cout << "DerivedObject constructed with derived_int_ = " << derived_int << std::endl;
		}
		//destructor included only to show order of destruction to verify pointer deletion is occurring correctly
		~DerivedObject() 
		{ 
			std::cout << "DerivedObject destructed" << std::endl;
		}


	protected:

	private:
		int derived_int_;

		//virtual function implementations
		inline void VirtualFunctionOne() const 
		{ 
			std::cout << "VirtualFunctionOne ran from DerivedObject" << std::endl; 
		}
		inline void VirtualFunctionTwo() const 
		{ 
			std::cout << "VirtualFunctionTwo ran from DerivedObject" << std::endl; 
		}
	};
}

int main()
{
	samples::DerivedObject derived_object = samples::DerivedObject(0, 1);
	derived_object.VirtualInterfaceFunction();

	using LO = samples::LargeObject; //just making the line below shorter and easier to read
	std::shared_ptr<LO> large_object_ptr = std::make_shared<LO>(LO({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 }));
	std::cout << large_object_ptr->int_at(11) << std::endl;

	derived_object.PassByRefFunction(*large_object_ptr);
	derived_object.PassByPtrFunction(large_object_ptr);
	derived_object.PassByValue(1);

	return 0;
}