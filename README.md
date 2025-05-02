# Password Manager

Use your flipper zero as a password manager and authenticator!      
The flipper stores your usernames and password and can write them on your PC acting as a keyboard!

## How does it work
- Add new credentials:
    - The flipper asks you to insert the website name, the username and the password
    - Credentials are stored into a file in the flipper *as plain text*; the Flipper Zero does not currently support a secure way to store your passwords
- See saved credentials:
    - The flipper shows a lists of website names
    - If OK is clicked, the flipper will write on the PC ```username + /t + password + /n```
        - *Perfect for web forms!!!*
- Delete credentials:
    - The flipper shows a lists of website names
    - If OK is clicked, the flipper will remove the selected line from the credentials file

## Add new password

![usage](img/add.gif)

## Web login

![usage](img/login.gif)

## Show passwords

![usage](img/passwords.gif)

## Delete passwords

![usage](img/delete.gif)

## WARNING
- If you lose the Flipper Zero, you should change the passwords on all sites that were stored. Credentials are stored *as plain text*.

