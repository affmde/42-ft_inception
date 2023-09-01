
<h1 align="center">42 - FT_IRC

<div align="center">
	<img alt="mac" src="https://img.shields.io/badge/mac%20os-000000?style=for-the-badge&logo=apple&logoColor=white"/>
	<img alt="Linux" src="https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black"/>
	<img alt="Ubuntu" src="https://img.shields.io/badge/Ubuntu-E95420?style=for-the-badge&logo=ubuntu&logoColor=white"/>
	<br>
	<img alt="c++" src="https://img.shields.io/badge/C++-00599C?style=flat-square&logo=C%2B%2B&logoColor=white"/>
	<br>
	<img alt="size" src="https://img.shields.io/github/languages/code-size/affmde/42-ft_irc"/>
	<img alt="total files" src="https://img.shields.io/github/directory-file-count/affmde/42-ft_irc"/>
	<img alt="repo size" src="https://img.shields.io/github/repo-size/affmde/42-ft_irc"/>
	<img alt="commits per month" src="https://img.shields.io/github/commit-activity/m/affmde/42-ft_irc"/>
	<img alt="last commit" src="https://img.shields.io/github/last-commit/affmde/42-ft_irc"/>
</div>

---

## **_Description_**
This project is about creating your own IRC server.
You will use an actual IRC client to connect to your server and test it.
Internet is ruled by solid standards protocols that allow connected computers to interact
with each other.
<br><br>

## **_Goals_**
Internet Relay Chat or IRC is a text-based communication protocol on the Internet.
It offers real-time messaging that can be either public or private. Users can exchange
direct messages and join group channels.
IRC clients connect to IRC servers in order to join channels. IRC servers are connected
together to form a network.
<br><br>

## **_Feactures_**

### _Supported commands:_ ###

| __Command__        | __Description__   |
| -------------  |:-------------:|
| PASS              | The PASS command is used to set a ‘connection password’. If set, the password must be set before any attempt to register the connection is made. |
| NICK       | The NICK command is used to give the client a nickname or change the previous one.      |
| USER  | The USER command is used at the beginning of a connection to specify the username and realname of a new user.      |
| CAP  | The CAP command is used for capability negotiation between a server and a client.      |
| INVITE  | The INVITE command is used to invite a user to a channel.      |
| JOIN  | The JOIN command indicates that the client wants to join the given channel(s), each channel using the given key for it.      |
| KICK  | The KICK command can be used to request the forced removal of a user from a channel. It causes the <user> to be removed from the <channel> by force.      |
| MODE  | The MODE command is used to set or remove options (or modes) from a given target.      |
| MOTD  | The MOTD command is used to get the “Message of the Day” of the given server. If <target> is not given, the MOTD of the server the client is connected to should be returned.      |
| NOTICE  | The NOTICE command is used to send notices between users, as well as to send notices to channels. <target> is interpreted the same way as it is for the PRIVMSG command.      |
| PART | The PART command removes the client from the given channel(s). On sending a successful PART command, the user will receive a PART message from the server for each channel they have been removed from. |
| PING | The PING command is sent by either clients or servers to check the other side of the connection is still connected and/or to check for connection latency, at the application layer. |
| PRIVMSG | The PRIVMSG command is used to send private messages between users, as well as to send messages to channels. <target> is the nickname of a client or the name of a channel. |
| QUIT | The QUIT command is used to terminate a client’s connection to the server. The server acknowledges this by replying with an ERROR message and closing the connection to the client. |
| TOPIC | The TOPIC command is used to change or view the topic of the given channel. If <topic> is not given, either RPL_TOPIC or RPL_NOTOPIC is returned specifying the current channel topic or lack of one. If <topic> is an empty string, the topic for the channel will be cleared. |

<br><br>

## **_Requirements_**
As this program is coded in C++, you will need a proper compiler.
It is recommended the use of an IRC client to connect to the server. Irssi is a good option.
You can also use the nc command, but the set up makes it more complicated to use.
<br><br>

## **_How to run it_**
To run the mandatory part just

```
$ git clone https://github.com/affmde/42-ft_irc.git
$ cd ft_irc
$ make && ./ircserv [port] [password]

```

replace port with the desirable port to connect.<br>
replace password with the desirable password to the server.<br>

Use an irc client, to connect to server. We use here Irssi.
Example of connecting with irssi.

```
$ make && ./ircserv 6667 hive
$ irssi -c localhost -w hive -n andrferr
```
## Commands to know about irssi: ##
 - -c the ip adress you want to connect to. In this case localhost or 127.0.0.1.
 - -p The port you want to connect. No required if the port is the default 6667.
 - -w The password required by the server.
 - -n The NICK you want to register with.
<br><br>

## **_Collaborators_**

[@hansielneff](https://github.com/hansielneff)
