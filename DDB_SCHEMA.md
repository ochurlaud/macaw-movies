# Here is the last DDB schema

**_Not the last version TO BE UPDATED_**

## movies
| Column Name   | Type | Link |
| ------------- | ---- | ---- |
| id | INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE | **1**|
| title | VARCHAR(255) NOT NULL | |
| original_title | VARCHAR(255) | |
| release_date | VARCHAR(10) | |
| country | VARCHAR(50) | |
| duration | INTEGER | |
| synopsys | TEXT | |
| file_path | VARCHAR(255) UNIQUE NOT NULL | |
| colored | BOOLEAN  | |
| format | VARCHAR(10) | |
| suffix | VARCHAR(10) | |
| rank | INTEGER | |

_Note: The longest movie title is 196 char:_ Night of the Day of the Dawn of the Son of the Bride of the Return of the Revenge of the Terror of the Attack of the Evil, Mutant, Hellbound, Flesh-Eating Subhumanoid Zombified Living Dead, Part 3

## people
| Column Name   | Type | Link |
| ------------- | ---- | ---- |
| id | INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE | **2** |
| lastname | VARCHAR(100)  NOT NULL | |
| firstname | VARCHAR(100) | |
| realname | VARCHAR(255) | |
| birthday |  VARCHAR(10) | |
| biography | TEXT | |

## movies_people
| Column Name   | Type | Link |
| ------------- | ---- | ---- |
| id | INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE | |
| id_movie | INTEGER  NOT NULL | **1** |
| id_people | INTEGER  NOT NULL | **2** |
| type | INTEGER  NOT NULL | |

`UNIQUE (id_people, id_movie, type) ON CONFLICT IGNORE`

## tags
| Column Name   | Type | Link |
| ------------- | ---- | ---- |
| id | INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE | **3** |
| name | VARCHAR(255) UNIQUE NOT NULL | |

## movies_tags
| Column Name   | Type | Link |
| ------------- | ---- | ---- |
| id | INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE | |
| id_movie | INTEGER NOT NULL | **1** |
| id_tag | INTEGER NOT NULL | **3** |

`UNIQUE (id_tag, id_movie) ON CONFLICT IGNORE`

## paths_list
| Column Name   | Type | Link |
| ------------- | ---- | ---- |
| id | INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE | |
| movies_path | VARCHAR(255) UNIQUE |  |

## config
| Column Name   | Type | Link |
| ------------- | ---- | ---- |
| db_version | INTEGER |  |

