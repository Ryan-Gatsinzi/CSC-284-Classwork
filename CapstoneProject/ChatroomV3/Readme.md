# C++ Curses Chatroom Application

A terminal-based multi-room chat application built in C++ using curses.

---

# Connecting to a Server

To join a chat server as a client, launch the client executable with:

```bash
./client <username> <server_ip> <port>
```

Example:

```bash
./client rayray 127.0.0.1 8080
```

### Arguments

| Argument      | Description                            |
| ------------- | -------------------------------------- |
| `<username>`  | Your display name in the chat          |
| `<server_ip>` | IP address of the server to connect to |
| `<port>`      | Server port number                     |

### Example Connection

```bash
./client john 192.168.1.50 9000
```

This connects the user `john` to the server running at `192.168.1.50` on port `9000`.

When connected successfully, the user is automatically placed into the `Lobby` room.

---

# Controls

## Chat Navigation

Use the arrow keys to navigate through chat history and room lists.

| Key   | Action                                       |
| ----- | -------------------------------------------- |
| ↑ / ↓ | Scroll through chat messages or room list    |
| ← / → | Switch focus between chat view and room list |
| ENTER | Join selected room from room list            |

---

# Chat Modes

The client has two navigation modes:

## 1. Chat Mode

Default mode used for typing and sending messages.

* You can type normally
* Messages are sent to the current room
* Any text not starting with a command is treated as a chat message

## 2. Scroll Mode

Activated when navigating chat history or room lists.

* Typing is disabled while scrolling through room
* Arrow keys are used for navigation
* Left/right arrows switch focus between:

  * Chat history
  * Room list

---

# Commands

## `.CREATE_ROOM <room_name>`

Creates a new chatroom on the server.

Example:

```txt
.CREATE_ROOM gaming
```

---

## `.JOIN_ROOM <room_name>`

Joins an existing room.

Example:

```txt
.JOIN_ROOM gaming
```

### Notes

* Users are automatically placed in the `Lobby` when first connecting
* Joining another room automatically removes the user from the previous room

---

## `.LIST_ROOMS`

Toggles the room list panel on and off.

While the room list is open:

* Use ↑ and ↓ to navigate rooms
* Press ENTER to join the selected room
* Use ← and → to switch between room list and chat history focus

---

## `.EXIT`

Disconnects cleanly from the server and exits the application.

For Client:

```txt
.EXIT
```
For Server:

```txt
quit
```

---

# Messaging

Any input that does not begin with a command is treated as a normal chat message and sent to the user's current room.

Example:

```t
```
