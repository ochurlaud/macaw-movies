# Here is the last DDB schema

## movies
| Column Name   | Type | Link |
| ------------- | ---- | ---- |
| id | INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE | **1**|
| title | VARCHAR(255) | |
| original_title | VARCHAR(255) | |
| year | INTEGER | |
| country | VARCHAR(50) | |
| duration | INTEGER | |
| synopsys | TEXT | |
| file_path | VARCHAR(255) | |
| colored | BOOLEAN  | |
| format | VARCHAR(10) | |
| suffix | VARCHAR(10) | |
| rank | INTEGER | |

_Note: The longest movie title is 196 char:_ Night of the Day of the Dawn of the Son of the Bride of the Return of the Revenge of the Terror of the Attack of the Evil, Mutant, Hellbound, Flesh-Eating Subhumanoid Zombified Living Dead, Part 3

## people
| Column Name   | Type | Link |
| ------------- | ---- | ---- |
| id | INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE | **2** |
| name | VARCHAR(100) | |
| first_name | VARCHAR(100) | |
| realname | VARCHAR(255) | |
| birthday | INTEGER | |
| biography | TEXT | |

## people_movies
| Column Name   | Type | Link |
| ------------- | ---- | ---- |
| id | INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE | |
| id_people | INTEGER | **2** |
| id_movie | INTEGER | **1** |
| type | INTEGER | |

## tags
| Column Name   | Type | Link |
| ------------- | ---- | ---- |
| id | INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE | **3** |
| tag_name | VARCHAR(255) | |


## tags_movies
| Column Name   | Type | Link |
| ------------- | ---- | ---- |
| id | INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE | |
| id_tag | INTEGER | **3** |
| id_movie | INTEGER | **1** |


## paths_list
| Column Name   | Type | Link |
| ------------- | ---- | ---- |
| id | INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE | |
| movies_path | VARCHAR(255) |  |

## config
| Column Name   | Type | Link |
| ------------- | ---- | ---- |
| db_version | INTEGER |  |

