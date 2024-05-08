#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

class Destination {
	
	protected:
		
		string cities[4] = {"Peshawar", "Islamabad", "Lahore", "Karachi"};
	
	
	public:
    	
		virtual ~Destination() {}
		
		bool setCity(string &city) 
		{
        
		int i = 0;

        while (i<4 && city != cities[i]) 
		{
            ++i;
        }

        if(i<4) 
		{
            city = cities[i];
            
            return true;
        }

        return false;
    }

    void displayAvailableDestinations() 
	{
        cout<<"\n\nAvailable Destinations:\n\n";

        int i = 0;

        while (i<4) 
		{
            cout<<i + 1<<". "<<cities[i]<<"\n\n";
            ++i;
        }
        cout<<"\n";
    }
};

class Passenger {
	protected:
    	string name;
    	char seatRow;
    	int seatNumber;
	public:
    	Passenger(string n, char row, int number) : name(n), seatRow(row), seatNumber(number) {}
};

class Transport {

	public:
    	
		virtual ~Transport() {}

    	virtual void book() = 0;
    	
		virtual void showSeats() = 0;
    	
		virtual double calculatePrice(int seatType, int numPassengers) const = 0;

};

class AvailabilityTracker {
	
	private:
    	bool flightSeats[7][5];
    	bool hotelRooms[10];

	public:
    	AvailabilityTracker() 
		{
        	for (int i = 0; i < 7; ++i)
            	for (int j = 0; j < 5; ++j)
                	flightSeats[i][j] = true;
					
			for (int i = 0; i < 10; ++i)
            	hotelRooms[i] = true;
    }

    bool isFlightSeatAvailable(char row, int seat) const 
	{
        return flightSeats[row - 'A'][seat - 1];
    }

    void bookFlightSeat(char row, int seat) 
	{
        flightSeats[row - 'A'][seat - 1] = false;
    }

    bool isHotelRoomAvailable(int roomNumber) const 
	{
        return hotelRooms[roomNumber - 1];
    }

    void bookHotelRoom(int roomNumber) 
	{
        hotelRooms[roomNumber - 1] = false;
    }
};

class Flight : public Destination, public Transport 
{
	private:
    	
		vector<Passenger> passengers;
    	
    	double businessClassPrice = 500.0;
    	
    	double economyClassPrice = 300.0;
    	
		AvailabilityTracker* availabilityTracker;

	public:
    	~Flight() {}

    	void setAvailabilityTracker(AvailabilityTracker* tracker) 
		{
        	availabilityTracker = tracker;
    	}

    	void book() override 
		{
        	int seatType;
        	
        	cout<<"\n\nFlight Booking:\n\n";

        	displayAvailableDestinations();

        	string userDestination;
			
			while (1) 
			{
            	cout<<"Enter your destination (select from the available options): ";
            	cin>>userDestination;
				
				if (setCity(userDestination)) 
				{

                	cout<<"You have selected " << userDestination << " as your destination.\n";
                	cout<<"Select seat type (1 for business, 2 for economy): ";
					
					
					cin >> seatType;
					
					if (seatType == 1) 
					{
                    	cout << "Business class selected. Price: $" << fixed << setprecision(2) << businessClassPrice << "\n";
                    	break;
                	} 
					
					else if (seatType == 2) 
					{
                    	cout << "Economy class selected. Price: $" << fixed << setprecision(2) << economyClassPrice << "\n";
                    	break;
                	}
					else 
					{
                    	cout << "Invalid choice.\n";
                    	continue;
                	}
            	}
				else 
				{
                	cout << "Invalid destination entered.\n";
                	continue;
            	}
        	}

        int numPassengers;
        
        cout<<"Enter the number of passengers: ";
        
        cin>>numPassengers;

        int i = 0;
        
        while(i<numPassengers) 
		{
            string name;
            
            char row;
            
            int number;

            cout<<"\n\nEnter passenger "<<i + 1<<"'s details:\n";
            
            cout<<"\nName: ";
            cin >> name;
            
            cout<<"\n\n";
            
            showSeats();
            
            cout<<"\n\n";

            cout<<"Enter the seat you want to book (e.g., A3): ";
            
            cin>>row>>number;

            if (isValidSeat(row, number) && availabilityTracker->isFlightSeatAvailable(row, number)) {
                
				cout<<"\n\nSeat "<<row<<number<<" has been booked for passenger " << name << "!\n\n";

                passengers.push_back(Passenger(name, row, number));
                availabilityTracker->bookFlightSeat(row, number);

                ++i;

            } 
			else 
			{
                cout<<"Invalid seat selection for passenger " << name <<", or the seat is already booked.\n";
            }
        }

        generateBill(seatType);
    }

    vector<Passenger> &getPassengers() 
	{
        return passengers;
    }

    void showSeats() override{
        
		cout<<"Available seats:\n";

        char row = 'A';

        while (row <= 'G') 
		{
            int seat = 1;

            while (seat <= 7) 
			{
                cout << row << seat << "  ";
                ++seat;
            }
            cout << "\n";
            ++row;
        }
    }

    bool isValidSeat(char row, int seat) 
	{
        return (row >= 'A' && row <= 'G') && (seat >= 1 && seat <= 5);
    }

    double calculatePrice(int seatType, int numPassengers) const override 
	{
        double additionalCharges = 0.0;
        
        double tax = 0.1 * (seatType == 1 ? businessClassPrice : economyClassPrice);
        
        return (seatType == 1 ? businessClassPrice : economyClassPrice) * numPassengers + additionalCharges + tax;
    }

    void generateBill(int seatType) 
	{
        double totalFlightAmount = calculatePrice(seatType, passengers.size());

        cout << "\n\n\nFlight Bill:\n\n\n";

        cout << "Base Fare per passenger: $ " << fixed << setprecision(2) << (seatType == 1 ? businessClassPrice : economyClassPrice) << "\n";

        cout << "\nTotal Passengers: " << passengers.size() << "\n";

        cout << "\nAdditional Charges: $" << fixed << setprecision(2) << 0.0 << "\n";

        cout << "\nTax (10%): $" << fixed << setprecision(2) << 0.1 * (seatType == 1 ? businessClassPrice : economyClassPrice) << "\n";

        cout << "\nTotal Flight Amount: $" << fixed << setprecision(2) << totalFlightAmount << "\n";
    }
};

class Hotel : public Destination, public Transport {
private:
    int singleRoomPrice = 99;
    int doubleRoomPrice = 199;
    int suiteRoomPrice = 299;
    AvailabilityTracker* availabilityTracker;

public:
    ~Hotel() {}

    void setAvailabilityTracker(AvailabilityTracker* tracker) {
        availabilityTracker = tracker;
    }

    void book() override {
        cout << "\n\nWelcome to Hotel Booking (In Association With Pearl Continental Hotels):\n";

        while (1) {
            string userDestination;

            displayAvailableDestinations();

            cout << "Enter your destination (select from the available options): ";
            cin >> userDestination;

            if (setCity(userDestination)) {
                cout << "You have selected " << userDestination << " as your destination.\n";
                string roomType[3] = {"single", "double", "suite"};

                int suiteRooms = 2;
                int doubleRooms = 5;
                int singleRooms = 10;

                cout << "\nSelect type of room (single/double/suite): ";

                string room;
                cin >> room;

                if (room == roomType[0]) {
                    cout << "\n\nAvailable rooms for " << roomType[0] << " rooms:\n";
                    int i = 1;
                    while (i <= 10) {
                        if (availabilityTracker->isHotelRoomAvailable(i)) {
                            cout << roomType[0] << " Room " << i << " - $" << singleRoomPrice << " per night\n";
                        }
                        ++i;
                    }
                } else if (room == roomType[1]) {
                    cout << "\n\nAvailable rooms for " << roomType[1] << " rooms:\n";
                    int i = 1;
                    while (i <= 5) {
                        if (availabilityTracker->isHotelRoomAvailable(i)) {
                            cout << roomType[1] << " Room " << i << " - $" << doubleRoomPrice << " per night\n";
                        }
                        ++i;
                    }
                } else if (room == roomType[2]) {
                    cout << "\n\nAvailable rooms for " << roomType[2] << " rooms:\n";
                    int i = 1;
                    while (i <= 2) {
                        if (availabilityTracker->isHotelRoomAvailable(i)) {
                            cout << roomType[2] << " Room " << i << " - $" << suiteRoomPrice << " per night\n";
                        }
                        ++i;
                    }
                } else {
                    cout << "Invalid room type.\n";
                    continue;
                }

                int numRooms;
                cout << "Enter the number of rooms: ";
                cin >> numRooms;

                double totalHotelAmount = 0.0;
                int j = 0;

                while (j < numRooms) {
                    int roomNumber;

                    cout << "Enter the room number you want to book: ";
                    cin >> roomNumber;

                    if (isValidRoom(roomNumber) && availabilityTracker->isHotelRoomAvailable(roomNumber)) {
                        cout << room << " Room " << roomNumber << " has been booked!\n";
                        totalHotelAmount += calculatePrice(roomNumber, 1);
                        availabilityTracker->bookHotelRoom(roomNumber);
                        ++j;
                    } else {
                        cout << "Invalid room selection or the room is already booked.\n";
                    }
                }

                generateBill(totalHotelAmount);
                break;
            } else {
                cout << "Invalid destination entered.\n";
                continue;
            }
        }
    }

    void showSeats() override {
        
    }

    double calculatePrice(int roomType, int numRooms) const override {
        double additionalCharges = 0.0;
        double tax = 0.1 * singleRoomPrice;

        switch (roomType) {
            case 1: // For single room;
                return singleRoomPrice * numRooms + additionalCharges + tax;

            case 2: // For double room;
                return doubleRoomPrice * numRooms + additionalCharges + tax;

            case 3: // For suite room;
                return suiteRoomPrice * numRooms + additionalCharges + tax;

            default:
                return 0.0; 
        }
    }

    void generateBill(double totalHotelAmount) {
        double additionalCharges = 0.0;
        double tax = 0.1 * totalHotelAmount;
        double totalAmount = totalHotelAmount + additionalCharges + tax;

        cout << "\n\n\nHotel Bill:\n\n\n";

        cout << "Room Charge per room: $100\n";

        cout << "Total Rooms: " << totalHotelAmount / 100 << "\n";

        cout << "Additional Charges: $" << fixed << setprecision(2) << additionalCharges << "\n";

        cout << "Tax (10%): $" << fixed << setprecision(2) << tax << "\n";

        cout << "Total Hotel Amount: $" << fixed << setprecision(2) << totalAmount << "\n";
    
    }

    bool isValidRoom(int roomNumber) {
        return roomNumber >= 1 && roomNumber <= 10;
    }
};



int main() 
{
    cout<<"\t\t\tWelcome to the Trip Planner PRO!";

    Destination destination;
    AvailabilityTracker availabilityTracker;

    int choice;

    while (1) 
	{
        cout<<"\n\nPress 1 for flights, press 2 for hotel, press 3 to exit: ";
        
        cin>>choice;

        if (choice == 3) 
		{
            cout<<"\nExiting the program.\n";
            break;
        }

        Transport *booking;

        if (choice == 1) 
		{
            booking = new Flight();
            
            dynamic_cast<Flight*>(booking)->setAvailabilityTracker(&availabilityTracker);
            
            booking->book();

            string choice1;
            cout<<"\n\nDo you also want to book a hotel? (yes/no): ";
            
            cin >> choice1;

            if (choice1 == "yes") 
			{
                booking = new Hotel();
                dynamic_cast<Hotel*>(booking)->setAvailabilityTracker(&availabilityTracker);
				booking->book();
            } 
			
			else 
			{
                cout << "\n\nNo hotel booked\n\n";
            }
        } 
		
		else if (choice == 2) 
		{
            booking = new Hotel();
            
            dynamic_cast<Hotel*>(booking)->setAvailabilityTracker(&availabilityTracker);
            
            booking->book();
        } 
		
		else 
		{
            cout << "Invalid choice.\n";
            continue;
        }

        	double totalBill = booking->calculatePrice(choice, (choice == 1 ? dynamic_cast<Flight *>(booking)->getPassengers().size() : 1));
        	
			cout <<"\nTotal Bill: $ " << fixed << setprecision(2) << totalBill << "\n";
			
			delete booking;
    }

    return 0;
}
