#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

struct user {
    char phone[50];
    char ac[50];
    char password[50];
    float balance;
} usr,usr1;

int windowWidth = 800;
int windowHeight = 600;
int currentScreen = 0;
char phoneInput[50] = "";
char transinput[50]="";
int transindex=0;
char passwordInput[50] = "";
int phoneIndex = 0;
int passwordIndex = 0;
char balance[50];
char amount[50] = "";
int amountIndex = 0;

void drawText(float x, float y, const char *string) {
    glRasterPos2f(x, y);
    for (int i = 0; string[i]; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
    }
}

void drawButton(float x, float y, float width, float height, const char *label) {
    glBegin(GL_QUADS);
    glColor3f(0.3f, 0.5f, 0.8f); // Button color
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();

    glColor3f(1.0f, 1.0f, 0.1f); // Text color
    drawText(x + width / 2 - 40, y + height / 2 - 10, label);
}

int accountNumGen(char a[], char b[]) {
    int accountNum = rand() % 1000000;
    snprintf(b, 17, "%s%06d", a, accountNum);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    switch (currentScreen) {
        case 0:
            drawButton(300, 450, 200, 50, "Register");
            drawButton(300, 350, 200, 50, "Login");
            drawButton(300, 250, 200, 50, "Avail Loan");
            drawButton(300, 150, 200, 50, "Exit");
            break;

        case 1:
            drawText(100, 500, "Register Phone Number");
            drawText(100, 400, "Enter Mobile Number:");
            drawText(100, 350, phoneInput);
            drawButton(300, 200, 200, 50, "Next");
            break;

        case 2:
            drawText(100, 500, "Register Password");
            drawText(100, 400, "Enter Password:");
            char maskedPassword[50];
            for (int i = 0; i < passwordIndex; i++) {
                maskedPassword[i] = '*';
            }
            maskedPassword[passwordIndex] = '\0';
            drawText(100, 350, maskedPassword);
            drawButton(300, 200, 200, 50, "Submit");
            break;

        case 3:
            drawText(100, 500, "Login");
            drawText(100, 400, "Enter Phone Number:");
            drawText(100, 350, phoneInput);
            drawButton(300, 200, 200, 50, "Next");
            break;

        case 4:
            drawText(100, 500, "Enter Password:");
            char maskedLoginPassword[50];
            for (int i = 0; i < passwordIndex; i++) {
                maskedLoginPassword[i] = '*';
            }
            maskedLoginPassword[passwordIndex] = '\0';
            drawText(100, 400, maskedLoginPassword);
            drawButton(300, 200, 200, 50, "Login");
            break;

        case 5:
            drawText(100, 500, "Thank you for banking with us!");
            drawText(100, 400, "Account number successfully generated!");
            drawText(100, 200, usr.ac);
            break;

        case 6:
            drawText(100, 500, "Account already registered, please try again.");
            break;

        case 7:
            drawText(100, 500, "Invalid phone number.");
            break;

        case 8:
            drawText(100, 500, "Enter a strong password.");
            break;

        case 9:
            drawText(100, 500, "Invalid password.");
            break;

        case 10:
            drawText(100, 500, "Login successful!");
            break;

        case 11:
            drawButton(300, 450, 200, 50, "Check Balance");
            drawButton(300, 350, 200, 50, "Deposit");
            drawButton(300, 250, 200, 50, "Withdraw");
            drawButton(300, 150, 200, 50, "Transfer");
            drawButton(300, 50, 200, 50, "Exit");
            break;

        case 12:
            drawText(100, 500, "Balance:");
            snprintf(balance, sizeof(balance), "%.2f", usr.balance);
            drawText(100, 400, balance);
            break;

        case 13: // Deposit amount input
            drawText(100, 500, "Enter the amount to deposit:");
            drawText(100, 400, amount);
            drawButton(300, 200, 200, 50, "Next");
            break;

        case 14: // Deposit confirmation
            drawText(100, 500, amount);
            drawText(100, 400, "Deposited successfully.");
            drawButton(300, 200, 200, 50, "Continue");
            break;

        case 15: // Withdraw amount input
            drawText(100, 500, "Enter the amount to withdraw:");
            drawText(100, 400, amount);
            drawButton(300, 200, 200, 50, "Next");
            break;

        case 16: // Withdraw confirmation
            drawText(100, 500, amount);
            drawText(100, 400, "Withdrawn successfully.");
            drawButton(300, 200, 200, 50, "Continue");
            break;
        case 17: // Transfer portal
            drawText(100, 500, "Enter Phone Number to Transfer:");
            drawText(100, 400, transinput); // Display phone number input
            drawButton(300, 200, 200, 50, "Next");
            break;

        case 18: // Transfer amount input
            drawText(100, 500, "Enter the amount to be transferred:");
            drawText(100, 400, amount); // Display amount input
            drawButton(300, 200, 200, 50, "Submit");
            break;

        case 19: // Transfer successful
            drawText(100, 500, "Transfer completed successfully!");
            drawButton(300, 200, 200, 50, "Continue");
            break;

        case 20: // Insufficient balance
            drawText(100, 500, "Insufficient balance!");
            drawButton(300, 200, 200, 50, "Continue");
            break;

        case 21: // Account not found
            drawText(100, 500, "Account does not exist!");
            drawButton(300, 200, 200, 50, "Continue");
            break;


    }

    glFlush(); // Render the content
}

int checkForPhone(const char *phoneNum) {
    if (strlen(phoneNum) != 10) return 2; // Check if phone number is exactly 10 characters
    char filename[60];
    snprintf(filename, sizeof(filename), "%s.dat", phoneNum);
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        return 0; // Phone number not registered
    } else {
        fclose(fp);
        return 1; // Phone number already registered
    }
}

int checkForPassword(const char *pw) {
    int lc = 0, uc = 0, sc = 0;
    for (int i = 0; pw[i]; i++) {
        if (islower(pw[i])) lc++;
        else if (isupper(pw[i])) uc++;
        else if (!isalnum(pw[i])) sc++;
    }
    return (lc > 0 && uc > 0 && sc > 0) ? 1 : 0;
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        float newY = windowHeight - y;

        if (currentScreen == 0) {
            if (x >= 300 && x <= 500) {
                if (newY >= 450 && newY <= 500) {
                    currentScreen = 1; // Switch to Register Phone screen
                } else if (newY >= 350 && newY <= 400) {
                    currentScreen = 3; // Switch to Login screen
                } else if (newY >= 250 && newY <= 300) {
                    // Avail Loan functionality (not implemented yet)
                } else if (newY >= 150 && newY <= 200) {
                    exit(0); // Exit the application
                }
            }
        }

        if (currentScreen == 11) { // Post-login portal
            if (x >= 300 && x <= 500) {
                if (newY >= 450 && newY <= 500) {
                    snprintf(balance, sizeof(balance), "%.2f", usr.balance);
                    currentScreen = 12; // Switch to check balance
                } else if (newY >= 350 && newY <= 400) {

                    currentScreen = 13; // Switch to deposit
                } else if (newY >= 250 && newY <= 300) {

                    currentScreen = 15; // Switch to withdraw
                } else if (newY >= 150 && newY <= 200) {
                    strcpy(usr.phone,phoneInput);
                    currentScreen=17;// Transfer functionality (not implemented yet)
                } else if (newY >= 50 && newY <= 100) {
                    // Password change functionality (not implemented yet)
                }
            }
        }
        if(currentScreen==19)
        {
            currentScreen=11;
        }

        // Handle button click on Register Phone screen
        if (currentScreen == 1) {
            if (x >= 300 && x <= 500 && newY >= 200 && newY <= 250) {
                int l = checkForPhone(phoneInput);
                if (l == 2) {
                    currentScreen = 7; // Invalid phone number
                } else if (l == 0) {
                    currentScreen = 2; // Valid phone number, move to password input screen
                    strcpy(usr.phone, phoneInput);
                } else {
                    currentScreen = 6; // Phone number already registered
                }
            }
        }

        // Handle button click on Deposit screen
// Within the mouse function...

// Handle button click on Deposit screen
// Handle button click on Deposit screen
if (currentScreen == 13) {
    if (x >= 300 && x <= 500 && newY >= 200 && newY <= 250) {
        float amt;
        int result = sscanf(amount, "%f", &amt);

        if (result == 1 && amt > 0) { // Ensure sscanf succeeded
            usr.balance += amt; // Update balance

            // Prepare filename
            FILE *f = fopen(strcat(usr.phone,".dat"), "rb");
            if (f) {
                fclose(f);
            f = fopen((usr.phone), "wb");
                fwrite(&usr, sizeof(struct user), 1, f);
                fclose(f);
                currentScreen = 14; // Move to deposit confirmation
            } else {
                FILE *fp=fopen((usr.phone),"wb");
                fwrite(&usr,sizeof(struct user),1,fp);
                fclose(fp);
                currentScreen=14;
            }
        } else {
            drawText(100, 300, "Invalid deposit amount.");
        }
    }
    // Ensure to clear the amount buffer for the next transaction
amount[0] = '\0'; // Clear the amount input
amountIndex = 0;  // Reset input index
}

// Handle button click on Withdraw screen
if (currentScreen == 15) {
    if (x >= 300 && x <= 500 && newY >= 200 && newY <= 250) {
        float withdrawAmount;
        int result = sscanf(amount, "%f", &withdrawAmount);

        if (result == 1 && withdrawAmount > 0 && withdrawAmount <= usr.balance) {
            usr.balance -= withdrawAmount; // Update balance

            // Prepare filename

            FILE *f = fopen(strcat(usr.phone,".dat"), "rb");
            if (f) {
                    fclose(f);
                f = fopen((usr.phone), "wb");
                fwrite(&usr, sizeof(struct user), 1, f);
                fclose(f);
                currentScreen = 16; // Show withdrawal confirmation
            } else {
                FILE *fp=fopen((usr.phone),"wb");
                fwrite(&usr,sizeof(struct user),1,fp);
                fclose(fp);
                currentScreen=16;
            }
        } else if (withdrawAmount > usr.balance) {
            drawText(100, 300, "Insufficient funds.");
        } else {
            drawText(100, 300, "Invalid withdrawal amount.");
        }
    }
    amount[0] = '\0'; // Clear the amount input
amountIndex = 0;  // Reset input index

}




// Ensure to clear the amount buffer for the next transaction
amount[0] = '\0';
amountIndex = 0;
        // Handle button click on Register Password screen
        if (currentScreen == 2) {
            if (x >= 300 && x <= 500 && newY >= 200 && newY <= 250) {
                int l = checkForPassword(passwordInput);
                if (l == 1) {
                    strcpy(usr.password, passwordInput);
                    accountNumGen(usr.phone, usr.ac);
                    usr.balance = 0; // Initialize balance
                    char filename[60];
                    snprintf(filename, sizeof(filename), "%s.dat", usr.phone);
                    FILE *f = fopen(filename, "wb"); // Open file in binary write mode
                    fwrite(&usr, sizeof(struct user), 1, f);
                    fclose(f);
                    printf("Registration submitted with phone: %s and password: %s\n", usr.phone, usr.password);
                    currentScreen = 5; // Move to Thank You screen
                } else {
                    currentScreen = 8; // Weak password
                }
            }
        }

        // Handle button click on Login screen
        if (currentScreen == 3) {
            if (x >= 300 && x <= 500 && newY >= 200 && newY <= 250) {
                int l = checkForPhone(phoneInput);
                if (l == 1) {
                    currentScreen = 4; // Proceed to password entry screen
                } else {
                    currentScreen = 7; // Account not registered
                }
            }
        }



        // Handle button click on Password Entry screen for Login
        if (currentScreen == 4) {
            if (x >= 300 && x <= 500 && newY >= 200 && newY <= 250) {
                char filename[60];
                snprintf(filename, sizeof(filename), "%s.dat", phoneInput);
                FILE *fp = fopen(filename, "rb"); // Open file in binary read mode
                if (fp != NULL) {
                    fread(&usr, sizeof(struct user), 1, fp);
                    fclose(fp);
                    if (strcmp(passwordInput, usr.password) == 0) {
                        printf("Login successful!\n");
                        currentScreen = 10; // Login successful
                    } else {
                        currentScreen = 9; // Invalid password
                    }
                }
            }
        }

        // Handle continue button after deposit
        if (currentScreen == 14) {
            currentScreen = 11; // Go back to post-login portal
        }
        if(currentScreen==16)
        {
            currentScreen=11;
        }



        glutPostRedisplay(); // Redraw the window
    }
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) exit(0); // ESC to exit

    if (currentScreen == 1) { // Register Phone Number Screen
        if (key == 13) { // Enter key
            int l = checkForPhone(phoneInput);
            if (l == 2) {
                currentScreen = 7; // Invalid phone number
            } else if (l == 0) {
                currentScreen = 2; // Valid phone number, move to password input screen
                strcpy(usr.phone, phoneInput);
            } else {
                currentScreen = 6; // Phone number already registered
            }
        } else if (phoneIndex < sizeof(phoneInput) - 1 && isprint(key)) {
            phoneInput[phoneIndex++] = key;
            phoneInput[phoneIndex] = '\0'; // Null terminate the string
        }
    } else if (currentScreen == 2) { // Register Password Screen
        if (key == 13) { // Enter key
            int l = checkForPassword(passwordInput);
            if (l == 1) {
                strcpy(usr.password, passwordInput);
                accountNumGen(usr.phone, usr.ac);
                usr.balance = 0; // Initialize balance
                char filename[60];
                snprintf(filename, sizeof(filename), "%s.dat", usr.phone);
                FILE *f = fopen(filename, "wb"); // Open file in binary write mode
                fwrite(&usr, sizeof(struct user), 1, f);
                fclose(f);
                printf("Registration submitted with phone: %s and password: %s\n", usr.phone, usr.password);
                currentScreen = 5; // Move to Thank You screen
            } else {
                currentScreen = 8; // Weak password
            }
        } else if (key == 8 && passwordIndex > 0) { // Backspace key
            passwordInput[--passwordIndex] = '\0'; // Remove last character
        } else if (passwordIndex < sizeof(passwordInput) - 1 && isprint(key)) {
            passwordInput[passwordIndex++] = key;
            passwordInput[passwordIndex] = '\0'; // Null terminate the string
        }
    }else if(currentScreen==19)
    {
        currentScreen=11;
    }
     else if (currentScreen == 8) {
        currentScreen = 2; // Go back to password input
    } else if (currentScreen == 3) { // Login Screen
        if (key == 13) { // Enter key
            int l = checkForPhone(phoneInput);
            if (l == 1) {
                currentScreen = 4; // Proceed to password entry screen
            } else {
                currentScreen = 7; // Account not registered
            }
        } else if (phoneIndex < sizeof(phoneInput) - 1 && isprint(key)) {
            phoneInput[phoneIndex++] = key;
            phoneInput[phoneIndex] = '\0'; // Null terminate the string
        }
    } else if (currentScreen == 4) { // Password Entry Screen
        if (key == 13) { // Enter key
            char filename[60];
            snprintf(filename, sizeof(filename), "%s.dat", phoneInput);
            FILE *fp = fopen(filename, "rb"); // Open file in binary read mode
            if (fp != NULL) {
                fread(&usr, sizeof(struct user), 1, fp);
                fclose(fp);
                if (strcmp(passwordInput, usr.password) == 0) {
                    printf("Login successful!\n");
                    currentScreen = 10; // Login successful
                } else {
                    currentScreen = 9; // Invalid password
                }
            }
        } else if (key == 8 && passwordIndex > 0) { // Backspace key
            passwordInput[--passwordIndex] = '\0'; // Remove last character
        } else if (passwordIndex < sizeof(passwordInput) - 1 && isprint(key)) {
            passwordInput[passwordIndex++] = key;
            passwordInput[passwordIndex] = '\0'; // Null terminate the string
        }
    } else if (currentScreen == 10) {
        currentScreen = 11; // Go to post-login portal
    } else if (currentScreen == 13) { // Deposit amount input
        if (key == 13) { // Enter key
            // Trigger next action (handled in mouse)
        } else if (amountIndex < sizeof(amount) - 1 && isprint(key)) {
            amount[amountIndex++] = key;
            amount[amountIndex] = '\0';
        }
    }
     else if (currentScreen == 15) { // Withdraw amount input
        if (key == 13) { // Enter key
            // Trigger next action (handled in mouse)
        } else if (amountIndex < sizeof(amount) - 1 && isprint(key)) {
            amount[amountIndex++] = key;
            amount[amountIndex] = '\0';
        }
    }

    if (currentScreen == 17) { // Transfer Enter Phone Number
        if (key == 13) { // Enter key
            // Check account and transition to amount input
            FILE *fp1;
            char aphone[50];
            strcpy(aphone, transinput);
            strcat(aphone, ".dat"); // Ensure it has .dat extension
            fp1 = fopen(aphone, "r");
            if (fp1 != NULL) {
                fclose(fp1);
                currentScreen = 18; // Proceed to amount input screen
                amountIndex = 0; // Reset amount index for new input
                memset(amount, 0, sizeof(amount)); // Clear amount buffer
            } else {
                currentScreen = 21; // Account not found
            }
        } else if (transindex < sizeof(transinput) - 1 && isprint(key)) {
            transinput[transindex++] = key;
            transinput[transindex] = '\0'; // Null terminate the string
        }
    } else if (currentScreen == 18) { // Transfer Amount Input
        if (key == 13) { // Enter key
            // Proceed with the transfer logic
            float transferAmount;
            sscanf(amount, "%f", &transferAmount);

            if (transferAmount <= 0) {
                // Handle invalid amount input
                snprintf(amount, sizeof(amount), "Invalid amount");
            } else {
                // Read sender's account
                FILE *fp = fopen(strcat(usr.phone,".dat"), "r");
                if(fp!=NULL){
                fread(&usr, sizeof(struct user), 1, fp);
                fclose(fp);

                if (usr.balance >= transferAmount) {
                    // Update sender's account
                    usr.balance -= transferAmount;
                    fp = fopen(strcat(usr.phone,".dat"), "wb");
                    fwrite(&usr, sizeof(struct user), 1, fp);
                    fclose(fp);

                    // Read recipient's account
                    char recipientFile[60];
                    snprintf(recipientFile, sizeof(recipientFile), "%s.dat", transinput);
                    fp = fopen(recipientFile, "r");
                    struct user recipient;
                    fread(&usr1, sizeof(struct user), 1, fp);
                    fclose(fp);

                    // Update recipient's account
                    usr1.balance += transferAmount;
                    fp = fopen(recipientFile, "w");
                    fwrite(&usr1, sizeof(struct user), 1, fp);
                    fclose(fp);

                    currentScreen = 19; // Transfer successful
                } else {
                    currentScreen = 20; // Insufficient balance
                }
            }
                else{
                    FILE *fp = fopen(usr.phone, "r");
                }
            }
        } else if (amountIndex < sizeof(amount) - 1 && isprint(key)) {
            amount[amountIndex++] = key;
            amount[amountIndex] = '\0'; // Null terminate the string
        }
    }

    else if (amountIndex < sizeof(amount) - 1 && isprint(key)) {
        amount[amountIndex++] = key;
        amount[amountIndex] = '\0'; // Null terminate the string
    }




    glutPostRedisplay(); // Redraw the window
}

int main(int argc, char **argv) {
    srand(time(NULL)); // Seed random number generator
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Banking Application");
    gluOrtho2D(0, windowWidth, 0, windowHeight);

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);

    glClearColor(0.0f, 0.0f, 0.0f, 0.1f); // Background color
    glutMainLoop();
    return 0;
}
