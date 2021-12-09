# OnlineBanking
Online banking system in linux.

=========================================================================================================================================
<br/>
Limitations of this system-
<br/>
1. If an ill-formed field is entered then only an admin can reset them.
2. Accounts once deleted cannot be retrieved.
3. Account number of deleted accounts cannot be reassigned.
4. Password if forgotten by user can only be reset by admin.
<br/>
=========================================================================================================================================
<br/>
How to run the project-
<br/>
Extract this compressed project folder.
<br/>
Change directory to this project folder.For example - cd /home/user/urja/Desktop/miniproject
<br/>
Run the server.out file.
<br/>
Run the client.out file.
<br/>
==========================================================================================================================================
Server.c
******************************************************************************************************************************************
Functions 

1. singlewindow(int fd)

PARAMETERS- Client socket file Descriptor
RETURN VALUE- Void
DESCRIPTION- handles single user accounts and provides the following functionality
1. Deposit- deposit money into your account.
2. Withdraw- withdraw money from your account.Checks for negative balance
3. Balance Inquiry- check available balance
4. Password Change - change the current password.Asks for confirmation as well.
5. View Details- view user details and transaction details from latest to oldest.Transaction log holds upto 20 transactions.
6. Exit- exit from the system

2. jointwindow(int fd)

PARAMETERS- Client socket file Descriptor
RETURN VALUE- Void
DESCRIPTION- handles joint user accounts and provides the following functionality
1. Deposit- deposit money into your account.
2. Withdraw- withdraw money from your account.Checks for negative balance
3. Balance Inquiry- check available balance
4. Password Change - change the current password.Asks for confirmation as well.
5. View Details- view user details and transaction details from latest to oldest.Transaction log holds upto 20 transactions.
6. Exit- exit from the system

3. adminwindow(int fd)

PARAMETERS- Client socket file Descriptor
RETURN VALUE- Void
DESCRIPTION- handles admin accounts and provides the following functionality
1. Add- add an admin,single or joint account.
2. Delete- delete an admin,single or joint account.Permanent deletion
3. Modify- modify an admin,single or joint account.Transaction logs are not modified.
4. Search- search for admin,single or joint account details.
5. Exit- exit from the system.
********************************************************************************************************************************************
Client.c
********************************************************************************************************************************************
1. Get reply for server and show it to the user.
2. Read query and responses from user and send it to server.
3. Using getpass for taking a password as input from user.
*********************************************************************************************************************************************
Header Files
*********************************************************************************************************************************************
1. Summary.h

* Keeps count of unique account numbers allocated by the system.

2. Single.h

* Structure of the single user database.

3. Joint.h

* Structure of the joint user database.

4. Admin.h

* Structure of the admin database.

**********************************************************************************************************************************************
