using namespace std;
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include "imdb.h"
#include <stdio.h>
#include <string.h>

const char *const imdb::kActorFileName = "actordata";
const char *const imdb::kMovieFileName = "moviedata";

imdb::imdb(const string& directory)
{
  const string actorFileName = directory + "/" + kActorFileName;
  const string movieFileName = directory + "/" + kMovieFileName;
  
  actorFile = acquireFileMap(actorFileName, actorInfo);
  movieFile = acquireFileMap(movieFileName, movieInfo);
}



int compare_actor(const void* pair, const void *offset_ptr) {
  char *key = *((char **)((char *)pair + sizeof(void *)));
  char *in_array = *(char **)pair + *(int *)offset_ptr;
  return strcmp(key, in_array);
} 


bool imdb::good() const
{
  return !( (actorInfo.fd == -1) || 
	    (movieInfo.fd == -1) ); 
}




// you should be implementing these two methods right here... 
bool imdb::getCredits(const string& player, vector<film>& films) const {
  pair<void *, char*> actor;
  actor.first = (void *)actorFile;
  actor.second = (char *)&player[0];

  void *offset_array = ((char *)actorFile + sizeof(int));

  int *res = (int *)bsearch((void *)&actor, offset_array, *(int *)actorFile, sizeof(int), compare_actor);

  if(res == NULL) return false;


  int player_size = player.length();
  if(player_size % 2 == 0) player_size++;
  player_size++;

  char *player_info = (char *)actorFile + *res;
  short *films_n_ptr = (short *)(player_info + player_size);
  short num_films = *films_n_ptr;

  int *film_offsets;
  if((player_size + 2) % 4 == 2) {
    film_offsets = (int *)((char *)films_n_ptr + sizeof(int));
  } else {
    film_offsets = (int *)((char *)films_n_ptr + sizeof(short));
  }

  for(short i = 0; i < num_films; i++) {
    film current_film;
    int movie_offset = *((int *)((char *)film_offsets + i * sizeof(int)));
    char *film_address = (char *)movieFile + movie_offset;

    current_film.title = film_address;
    current_film.year = *(film_address + current_film.title.size() + 1) + 1900;

    films.push_back(current_film);
  }
  
  return true;
}



int compare_movie(const void *pair_ptr, const void *offset_ptr) {
  void* file = ((pair<void *, pair<string, int>> *)pair_ptr)->first;
  char *in_array = (char *)file + *(int *)offset_ptr;

  film key_film;
  film in_array_film;

  string key_name = ((pair<void *, pair<string, int>> *)pair_ptr)->second.first;
  string in_array_name = "";


  while(*in_array != '\0') {
    in_array_name += *in_array;
    in_array++;
  }

  key_film.title = key_name;
  in_array_film.title = in_array_name;

  in_array++;
  int years = *(char *)in_array;

  key_film.year = ((pair<void *, pair<string, int>> *)pair_ptr)->second.second;
  in_array_film.year = years + 1900;


  if (key_film == in_array_film) {
      return 0;
  } else if (key_film < in_array_film) {
      return -1;
  }
  return 1;
}


bool imdb::getCast(const film& movie, vector<string>& players) const {
  pair<void *, pair<string, int>> flick;
  flick.first = (void *)movieFile;
  flick.second.first = movie.title;
  flick.second.second = movie.year;

  void *offset_array = ((char *)movieFile + sizeof(int));

  int *res = (int *)bsearch((void *)&flick, offset_array, *(int *)movieFile, sizeof(int), compare_movie);

  if(res == NULL) return false;

  int film_size = movie.title.size();
  film_size += 2; //'\0' and char byte
  if(film_size % 2 == 1) film_size++;
  
  short num_actors = *((short *)((char *)movieFile + *res + film_size));
  
  if((film_size + 2) % 4 == 2) {
    film_size += 4;
  } else {
    film_size += 2;
  }
  
 int *actors_offsets_ptr = (int *)((char *)movieFile + *res + film_size);
 
 for(int i = 0; i < num_actors; i++) {
  char *address_in_actor_file = (char *)actorFile + *((int *)actors_offsets_ptr + i);
  string tmp = "";
  while(*address_in_actor_file != '\0') {
    tmp += *address_in_actor_file;
    address_in_actor_file++;
  }
  players.push_back(tmp);
 }

  return true; 
}

imdb::~imdb()
{
  releaseFileMap(actorInfo);
  releaseFileMap(movieInfo);
}

// ignore everything below... it's all UNIXy stuff in place to make a file look like
// an array of bytes in RAM.. 
const void *imdb::acquireFileMap(const string& fileName, struct fileInfo& info)
{
  struct stat stats;
  stat(fileName.c_str(), &stats);
  info.fileSize = stats.st_size;
  info.fd = open(fileName.c_str(), O_RDONLY);
  return info.fileMap = mmap(0, info.fileSize, PROT_READ, MAP_SHARED, info.fd, 0);
}

void imdb::releaseFileMap(struct fileInfo& info)
{
  if (info.fileMap != NULL) munmap((char *) info.fileMap, info.fileSize);
  if (info.fd != -1) close(info.fd);
}
