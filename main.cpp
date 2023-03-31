#include <iostream>
#include <utility>
#include "vector"
#include "algorithm"

using namespace std;

class Product {
public:
    string name;
    double price;

    Product(string name, double price) {
        this->name = std::move(name);
        this->price = price;
    }
};

class [[maybe_unused]] ProductAlreadyAddedException : public exception {
public:
    [[maybe_unused]] explicit ProductAlreadyAddedException(const char* message) : msg(message) {}

    [[nodiscard]] const char* what() const noexcept override {
        return msg.c_str();
    }

private:
    string msg;
};

class ProductNotFoundException : public exception {
public:
    explicit ProductNotFoundException(const char* message) : msg(message) {}

    [[nodiscard]] const char* what() const noexcept override {
        return msg.c_str();
    }

private:
    string msg;
};

string lowerString(string value) {
    for(char & i : value) {
        i = (char)tolower(i);
    }

    return value;
}

[[maybe_unused]] bool product_in_cart(string name, const vector<Product>& shopping_cart) {
    name = lowerString(name);
    return std::any_of(shopping_cart.begin(), shopping_cart.end(), [&](const Product& product) {
        return product.name == name;
    });
}

double product_get_price(string name, const vector<Product>& products) {
    name = lowerString(name);
    for(const auto & product : products) {
        if(lowerString(product.name) == name) {
            return product.price;
        }
    }

    throw ProductNotFoundException("Product not found.");
}

int main() {
    vector<Product> shopping_cart;
    vector<Product> products {
            {"Potato", 0.50},
            {"Tomato", 0.25},
            {"Apple", 0.39},
            {"Strawberry", 0.45},
    };

    char repeat;
    do {
        try {
            cout << "Add Product to shopping cart" << endl;
            cout << "\tName: ";
            string name;
            getline(cin, name);
            double price = product_get_price(name, products);
            cout << "\tPrice: " << price << endl;
            shopping_cart.emplace_back(name, price);
            cout << "Add new product to shopping cart? (Y / N): ";
            cin >> repeat;
            cin.ignore();
        } catch (...) {
            exception_ptr ex = current_exception();
            if(ex) {
                try {
                    rethrow_exception(ex);
                } catch (const exception& ex) {
                    cout << "Error: " << ex.what() << endl;
                }
            }
            repeat = 'y';
            continue;
        }
    } while(tolower(repeat) == 'y');

    cout << "You have " << shopping_cart.size() << " items in your shopping cart." << endl;
    double total = 0;
    for(auto & i : shopping_cart) {
        total += i.price;
    }
    cout << "Your total is " << total << endl;

    return 0;
}