#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <limits>

using namespace std;

// Basic unit representing a product in the catalog.
class Product {
public:
    string productCode, productName;
    double productPrice;
    Product(string c, string n, double p) 
        : productCode(c), productName(n), productPrice(p) {}
};

// Represents an individual order item (a product with its quantity).
class OrderItem {
public:
    Product item;
    int quantity;
    OrderItem(Product prod, int qty) 
        : item(prod), quantity(qty) {}
};

// Manages a collection of order items, tracking the total cost.
class ShoppingCart {
public:
    int cartID;
    vector<OrderItem> itemsList;
    double totalCost;
    ShoppingCart(int id) 
        : cartID(id), totalCost(0.0) {}

    // Adds a product to the cart.
    void addProduct(const Product &prod, int qty) {
        bool found = false;
        for (auto &entry : itemsList) {
            if (entry.item.productCode == prod.productCode) {
                entry.quantity += qty;
                found = true;
                break;
            }
        }
        if (!found)
            itemsList.push_back(OrderItem(prod, qty));
        totalCost += prod.productPrice * qty;
    }

    // Removes a product from the cart; if the removal quantity is 
    // equal to or exceeds current quantity, removes the item entirely.
    void removeProduct(const string &code, int qty) {
        for (size_t i = 0; i < itemsList.size(); i++) {
            if (itemsList[i].item.productCode == code) {
                if (qty >= itemsList[i].quantity) {
                    totalCost -= itemsList[i].item.productPrice * itemsList[i].quantity;
                    itemsList.erase(itemsList.begin() + i);
                } else {
                    itemsList[i].quantity -= qty;
                    totalCost -= itemsList[i].item.productPrice * qty;
                }
                break;
            }
        }
    }

    // Displays the contents of the cart.
    void displayCart(bool compact = false) const {
        if (itemsList.empty()) {
            cout << "Your cart is empty.\n";
            return;
        }
        cout << fixed << setprecision(2);
        if (compact)
            cout << "Product ID      Name      Price    Quantity" << "\n";
        else {
            cout << left << setw(15) << "Product ID"
                 << setw(15) << "Name"
                 << setw(10) << "Price"
                 << setw(10) << "Quantity"
                 << setw(12) << "Subtotal" << "\n";
            cout << string(62, '-') << "\n";
        }
        for (const auto &entry : itemsList) {
            if (compact)
                cout << left << setw(15) << entry.item.productCode
                     << setw(15) << entry.item.productName
                     << setw(10) << entry.item.productPrice
                     << setw(10) << entry.quantity << "\n";
            else
                cout << left << setw(15) << entry.item.productCode
                     << setw(15) << entry.item.productName
                     << setw(10) << entry.item.productPrice
                     << setw(10) << entry.quantity
                     << setw(12) << (entry.item.productPrice * entry.quantity) << "\n";
        }
        if (!compact)
            cout << "\nTotal Amount: Php " << totalCost << "\n";
    }

    bool isEmpty() const { return itemsList.empty(); }
    double getTotal() const { return totalCost; }
};

// Represents a finalized order containing a shopping cart.
class Order {
public:
    int orderNumber;
    ShoppingCart orderCart;
    Order(int num, const ShoppingCart &cart) 
        : orderNumber(num), orderCart(cart) {}

    // Displays the order information.
    void displayOrder() const {
        cout << "Order ID: " << orderNumber << "\n";
        cout << "Total Amount: Php " << orderCart.getTotal() << "\n";
        cout << "Order Details:" << "\n";
        orderCart.displayCart(true);
    }
};

// ========== Utility Functions ==========

// Prompts the user until a valid integer between low and high is entered.
int obtainValidInteger(const string &promptMsg, int minVal, int maxVal) {
    int userInput;
    while (true) {
        cout << promptMsg;
        if (cin >> userInput && userInput >= minVal && userInput <= maxVal) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return userInput;
        } else {
            cout << "Invalid input. Please try again.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

// Displays the order history.
void printOrderHistory(const vector<Order> &orderLog) {
    if (orderLog.empty()) {
        cout << "No previous orders available.\n";
        return;
    }
    cout << "\n*** Order History ***\n";
    for (const auto &ord : orderLog) {
        cout << "Order ID: " << ord.orderNumber << "\n";
        cout << "Total Amount: Php " << ord.orderCart.getTotal() << "\n";
        cout << "Order Details:" << "\n";
        ord.orderCart.displayCart(true);
        cout << "\n";
    }
}

// ========== Main Function ==========

int main() {
    // Initialize product catalog.
    vector<Product> products = {
        Product("BIG", "Bigas", 40.50),
        Product("SRD", "Sardinas", 15.75),
        Product("PNDS", "Pandesal", 30.00),
        Product("CRND", "Corned Beef", 80.00),
        Product("KAPE", "Kape", 120.00),
        Product("ITLG", "Itlog", 3.50)
    };

    int nextOrderIndex = 1;
    vector<Order> ordersRecord;
    ShoppingCart cart(1);
    bool continueApp = true;

    while (continueApp) {
        cout << "\n=== Shopping Menu ===\n";
        cout << "1. View Product List\n";
        cout << "2. Add Product to Cart\n";
        cout << "3. Remove Product from Cart\n";
        cout << "4. View Cart\n";
        cout << "5. Place Order\n";
        cout << "6. Order History\n";
        cout << "7. Exit\n";

        int option = obtainValidInteger("Select an option: ", 1, 7);
        if (option == 1) {
            cout << "\nAvailable Products:\n";
            for (const auto &prod : products)
                cout << "Product ID: " << prod.productCode << ", "
                     << "Name: " << prod.productName << ", "
                     << "Price: Php " << prod.productPrice << "\n";
        } else if (option == 2) {
            cout << "Enter the Product ID to add: ";
            string inputID;
            cin >> inputID;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            bool found = false;
            for (const auto &prod : products) {
                if (prod.productCode == inputID) {
                    int qty = obtainValidInteger("Enter quantity: ", 1, 100);
                    cart.addProduct(prod, qty);
                    cout << prod.productName << " added (" << qty << " units) to your cart.\n";
                    found = true;
                    break;
                }
            }
            if (!found)
                cout << "Product ID not recognized.\n";
        } else if (option == 3) {
            if (cart.isEmpty())
                cout << "Your cart is empty.\n";
            else {
                cart.displayCart();
                cout << "Enter the Product ID to remove: ";
                string removeID;
                cin >> removeID;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                int removeQty = obtainValidInteger("Enter quantity to remove: ", 1, 100);
                cart.removeProduct(removeID, removeQty);
                cout << "Removal completed.\n";
            }
        } else if (option == 4) {
            cart.displayCart();
        } else if (option == 5) {
            if (cart.isEmpty())
                cout << "Shopping cart is empty. Unable to place order.\n";
            else {
                Order newOrder(nextOrderIndex, cart);
                cout << "Order confirmed successfully!\n";
                newOrder.displayOrder();
                ordersRecord.push_back(newOrder);
                nextOrderIndex++;
                cart = ShoppingCart(1);
            }
        } else if (option == 6) {
            printOrderHistory(ordersRecord);
        } else if (option == 7) {
            continueApp = false;
            cout << "Exiting the shopping menu. Til next time!\n";
        }
    }
    return 0;
}