# BookShopInventory with socket
Client-Server version of project "BookShopInventory" (https://github.com/MazzuccaKristian/BookShopInventory).
Sockets allow a connection between client-side and server-side.
Client needs info from the archive; to get them, client makes a request and send it to the server. Server receives that query, executes it with the archive and sends back info.
At start-up time, the client-side establishes a connection via socket with the server side. Then, the user can select one option from the list in the main menu. Then, user fills fields with data. At this point, the client-side format a query and sends it to server.
Server-side is the receiving actor at the moment and receives client's query. Then, it uses part of that data to select the desidered action. Server performs the operation and sends back to client a query with the result.
The client-side must be closed from the user when its work is done. The server-side is always alive, because it could serve different connection from different clients.