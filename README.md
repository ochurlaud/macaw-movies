# Macaw-Movies

Movies organizer. Qt/C++/Sqlite

*This application is a movie organizer. It helps anyone to order, choose and find back a movie in his collection.*

*The program is written in C++, with the Qt framework. It will be released for Linux, Windows and Mac OS X*

1. Rules for coding
2. Presentation of the project

## 1. Rules for coding

See [Code rules](https://github.com/macaw-movies/macaw-movies/wiki/Code-rules) on the Wiki.

## 2. Presentation of the project

This application is a movie organizer. It only let the user order and find the movies he has; the movie is opened with the user's favorite tool.

We should be able do add several metadata to each movie. To make it easily, the application will get and fill theses data thanks to an Internet movie database (like gracenote for the music).

### Metadata to add 
* **Bold** means that it's implemented
* *Italic* means that the field exists but is not used
* Regular means that it has to be done


* **Title (I)**
* **Original title (I)**
* **Release date (I)**
* Type of movie (Drama, documentary, romance, ... - multiple choice (I)
* **Director (I)**
* **Country**
* **Synopsis**
* **Path to movie**, subtitle (sometimes several movie for part 1, 2)
* *Duration (read on file)*
* Language available so on the file
* *Color/B&W*
* *Format* (16/9, 4/3, etc) (read on the file)
* Image size (620x420, 1920x1080, etc (read on the file)
* Cover
* Writer
* **Actors (I)**
* **Producer (I)**
* Music by
* official website
* Box office
* budget
* Profit when released
* Nb of spectators when released
* Prizes: Oscar, lion, etc (several possibilities) And for whom  (actor, actress, producer, director, writer, etc) (I)
* Shooting places (several possibilities: Lyon, Paris, London....)

### Complement for series
* Season
* Name of the episode
* Number of the episode


**All the (I) are indexes for ordering or research.**
