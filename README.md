# Banking System in C with OpenGL

## Overview
This project is a simple banking system developed in the C programming language using OpenGL for the graphical user interface. It simulates basic banking operations with a user-friendly graphical interface controlled through keyboard and mouse interactions.

## Features
- **User Registration**  
  Create a new account by providing basic details.

- **Login Authentication**  
  Secure login for registered users.

- **Balance Inquiry**  
  View the current account balance.

- **Deposit Money**  
  Add funds to the user’s account.

- **Withdraw Money**  
  Withdraw funds with balance checks.

- **Fund Transfer**  
  Transfer money to another registered user.

## Technologies Used
- **C Programming Language**  
- **OpenGL (GLUT)** for GUI  
- **File Handling** using `.dat` files to store and retrieve user information

## How It Works
All user data (such as username, password, balance) is stored locally in `.dat` files. The interface includes buttons and menus rendered using OpenGL, enabling users to interact with the system via mouse clicks and keyboard inputs.

## How to Run
1. Ensure you have a C compiler and OpenGL/GLUT libraries installed.
2. Compile the project using a compiler like `gcc`:

## Future Improvements
- Add encryption for stored user data
- Improve UI/UX design
- Add support for transaction history
- Port to modern GUI frameworks (e.g., Qt)
