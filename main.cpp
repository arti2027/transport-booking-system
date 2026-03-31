#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

// Base class for vehicles
class Vehicle {
public:
    int vehicleNumber;
    std::string source;
    std::string destination;
    int totalSeats;
    int availableSeats;
    float fare;

    Vehicle(int num, std::string src, std::string dest, int seats, float fr)
        : vehicleNumber(num), source(src), destination(dest), totalSeats(seats), availableSeats(seats), fare(fr) {}
};

// Derived classes
class Bus : public Vehicle {
public:
    Bus(int num, std::string src, std::string dest, int seats, float fr)
        : Vehicle(num, src, dest, seats, fr) {}
};

class Train : public Vehicle {
public:
    Train(int num, std::string src, std::string dest, int seats, float fr)
        : Vehicle(num, src, dest, seats, fr) {}
};

class Flight : public Vehicle {
public:
    Flight(int num, std::string src, std::string dest, int seats, float fr)
        : Vehicle(num, src, dest, seats, fr) {}
};

// Passenger class
class Passenger {
public:
    std::string name;
    int age;
    int seatNumber;
    int vehicleNumber;
    std::string vehicleType; // "Bus", "Train", "Flight"

    Passenger(std::string n, int a, int seat, int vnum, std::string vtype)
        : name(n), age(a), seatNumber(seat), vehicleNumber(vnum), vehicleType(vtype) {}
};

// Booking system class
class BookingSystem {
private:
    std::vector<Bus> buses;
    std::vector<Train> trains;
    std::vector<Flight> flights;
    std::vector<Passenger> passengers;
    const std::string dataFile = "reservation_data.txt";

    void loadData() {
        std::ifstream file(dataFile);
        if (!file) return;
        std::string line;
        while (std::getline(file, line)) {
            // Parse line: name,age,seat,vehicleNumber,vehicleType
            size_t pos = 0;
            std::string token;
            std::vector<std::string> tokens;
            while ((pos = line.find(',')) != std::string::npos) {
                token = line.substr(0, pos);
                tokens.push_back(token);
                line.erase(0, pos + 1);
            }
            tokens.push_back(line);
            if (tokens.size() == 5) {
                passengers.emplace_back(tokens[0], std::stoi(tokens[1]), std::stoi(tokens[2]), std::stoi(tokens[3]), tokens[4]);
                // Update available seats
                updateAvailableSeats(tokens[4], std::stoi(tokens[3]), -1);
            }
        }
        file.close();
    }

    void saveData() {
        std::ofstream file(dataFile);
        for (const auto& p : passengers) {
            file << p.name << "," << p.age << "," << p.seatNumber << "," << p.vehicleNumber << "," << p.vehicleType << "\n";
        }
        file.close();
    }

    void updateAvailableSeats(const std::string& type, int num, int delta) {
        if (type == "Bus") {
            for (auto& b : buses) {
                if (b.vehicleNumber == num) {
                    b.availableSeats += delta;
                    break;
                }
            }
        } else if (type == "Train") {
            for (auto& t : trains) {
                if (t.vehicleNumber == num) {
                    t.availableSeats += delta;
                    break;
                }
            }
        } else if (type == "Flight") {
            for (auto& f : flights) {
                if (f.vehicleNumber == num) {
                    f.availableSeats += delta;
                    break;
                }
            }
        }
    }

    Vehicle* findVehicle(const std::string& type, int num) {
        if (type == "Bus") {
            for (auto& b : buses) {
                if (b.vehicleNumber == num) return &b;
            }
        } else if (type == "Train") {
            for (auto& t : trains) {
                if (t.vehicleNumber == num) return &t;
            }
        } else if (type == "Flight") {
            for (auto& f : flights) {
                if (f.vehicleNumber == num) return &f;
            }
        }
        return nullptr;
    }

public:
    BookingSystem() {
        // Initialize sample data with Indian cities and routes
        buses.emplace_back(101, "Delhi", "Goa", 50, 1500.0f);
        buses.emplace_back(102, "Mumbai", "Goa", 40, 1200.0f);
        buses.emplace_back(103, "Delhi", "Mumbai", 60, 800.0f);
        trains.emplace_back(201, "Delhi", "Goa", 100, 2500.0f);
        trains.emplace_back(202, "Mumbai", "Goa", 80, 2200.0f);
        trains.emplace_back(203, "Delhi", "Chennai", 120, 3000.0f);
        flights.emplace_back(301, "Delhi", "Goa", 200, 8000.0f);
        flights.emplace_back(302, "Mumbai", "Goa", 150, 7500.0f);
        flights.emplace_back(303, "Delhi", "Bangalore", 180, 6000.0f);
        loadData();
    }

    ~BookingSystem() {
        saveData();
    }

    void displayMainMenu() {
        std::cout << "\n=== Ticket Reservation System ===\n";
        std::cout << "1. Book a Ticket\n";
        std::cout << "2. Cancel a Ticket\n";
        std::cout << "3. Check Vehicle Status and List\n";
        std::cout << "4. Show Reserved Tickets\n";
        std::cout << "5. Exit\n";
        std::cout << "Enter your choice: ";
    }

    void searchAndBook() {
        std::string from, to, type;
        std::cout << "Enter departure city: ";
        std::cin >> from;
        std::cout << "Enter destination city: ";
        std::cin >> to;
        std::cout << "Enter vehicle type (Bus/Train/Flight): ";
        std::cin >> type;

        std::vector<Vehicle*> available;
        if (type == "Bus" || type == "All") {
            for (auto& b : buses) {
                if (b.source == from && b.destination == to && b.availableSeats > 0) available.push_back(&b);
            }
        }
        if (type == "Train" || type == "All") {
            for (auto& t : trains) {
                if (t.source == from && t.destination == to && t.availableSeats > 0) available.push_back(&t);
            }
        }
        if (type == "Flight" || type == "All") {
            for (auto& f : flights) {
                if (f.source == from && f.destination == to && f.availableSeats > 0) available.push_back(&f);
            }
        }

        if (available.empty()) {
            std::string plural_type = (type == "Bus") ? "Buses" : type + "s";
            std::cout << "No available " << plural_type << " from " << from << " to " << to << ".\n";
            return;
        }

        std::cout << "Available options:\n";
        for (size_t i = 0; i < available.size(); ++i) {
            std::cout << i+1 << ". " << type << " " << available[i]->vehicleNumber
                      << " - Seats: " << available[i]->availableSeats << "/" << available[i]->totalSeats
                      << " - Fare: " << available[i]->fare << "\n";
        }

        int choice;
        std::cout << "Choose an option by number (1-" << available.size() << ") or enter vehicle number: ";
        std::cin >> choice;

        Vehicle* selected = nullptr;
        if (choice >= 1 && choice <= (int)available.size()) {
            selected = available[choice - 1];
        } else {
            for (auto* v : available) {
                if (v->vehicleNumber == choice) {
                    selected = v;
                    break;
                }
            }
        }

        if (!selected) {
            std::cout << "Invalid choice.\n";
            return;
        }
        std::string name;
        int age;
        std::cout << "Enter Passenger Name: ";
        std::cin.ignore();
        std::getline(std::cin, name);
        std::cout << "Enter Passenger Age: ";
        std::cin >> age;

        int seatNumber = selected->totalSeats - selected->availableSeats + 1;
        passengers.emplace_back(name, age, seatNumber, selected->vehicleNumber, type);
        selected->availableSeats--;

        std::cout << "Ticket booked successfully! Seat: " << seatNumber << ", Fare: " << selected->fare << "\n";
    }

    void cancelTicket() {
        std::string name;
        std::cout << "Enter Passenger Name: ";
        std::cin.ignore();
        std::getline(std::cin, name);

        auto it = std::find_if(passengers.begin(), passengers.end(),
            [&name](const Passenger& p) { return p.name == name; });
        if (it == passengers.end()) {
            std::cout << "Passenger with name " << name << " not found.\n";
            return;
        }

        updateAvailableSeats(it->vehicleType, it->vehicleNumber, 1);
        passengers.erase(it);
        std::cout << "Ticket canceled successfully!\n";
    }

    void checkStatus() {
        std::string type;
        std::cout << "Enter vehicle type (Bus/Train/Flight/All): ";
        std::cin >> type;
        if (type == "All" || type == "Bus") {
            std::cout << "\nBuses:\n";
            for (const auto& b : buses) {
                std::cout << "Number: " << b.vehicleNumber << ", " << b.source << " to " << b.destination
                          << ", Available: " << b.availableSeats << "/" << b.totalSeats << ", Fare: " << b.fare << "\n";
            }
        }
        if (type == "All" || type == "Train") {
            std::cout << "\nTrains:\n";
            for (const auto& t : trains) {
                std::cout << "Number: " << t.vehicleNumber << ", " << t.source << " to " << t.destination
                          << ", Available: " << t.availableSeats << "/" << t.totalSeats << ", Fare: " << t.fare << "\n";
            }
        }
        if (type == "All" || type == "Flight") {
            std::cout << "\nFlights:\n";
            for (const auto& f : flights) {
                std::cout << "Number: " << f.vehicleNumber << ", " << f.source << " to " << f.destination
                          << ", Available: " << f.availableSeats << "/" << f.totalSeats << ", Fare: " << f.fare << "\n";
            }
        }
    }

    void showReservedTickets() {
        if (passengers.empty()) {
            std::cout << "\nNo reserved tickets.\n";
        } else {
            std::cout << "\nReserved Tickets:\n";
            for (const auto& p : passengers) {
                std::cout << "Name: " << p.name << ", Age: " << p.age << ", Seat: " << p.seatNumber
                          << ", " << p.vehicleType << " Number: " << p.vehicleNumber << "\n";
            }
        }
    }
};

int main() {
    BookingSystem system;
    int choice;
    do {
        system.displayMainMenu();
        std::cin >> choice;
        switch (choice) {
            case 1:
                system.searchAndBook();
                break;
            case 2:
                system.cancelTicket();
                break;
            case 3:
                system.checkStatus();
                break;
            case 4:
                system.showReservedTickets();
                break;
            case 5:
                std::cout << "Exiting...\n";
                break;
            default:
                std::cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 5);
    return 0;
}