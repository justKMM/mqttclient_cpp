# MQTTProject

## Starter Guide
1. Clone this repo into your chosen folder.
2. Clone this [EmbSysLib repo](https://github.com/ThBreuer/EmbSysLib) from Prof. Thomas Breuer into the same folder.
3. Compile the project / use any IDE which supports C++. You may have to install the GNU GCC compiler manually, or some IDEs may help you with this step.

## Commands (and their parameters)
- connect
  + client_id (NOT NULL)
  + username
  + password
- subscribe
  + topic (NOT NULL)
- publish
  + topic (NOT NULL)
  + message (NOT NULL)
