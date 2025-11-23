# RSS News Search Engine

A C implementation of a news article indexing and search engine that crawls RSS feeds, downloads articles, and builds an inverted index for fast keyword-based search across multiple news sources.

## Overview

This project demonstrates:

- RSS feed parsing and article extraction
- Web scraping using libcurl for HTTP requests
- HTML parsing to extract article text
- Inverted index construction using custom hashset and vector implementations
- Multi-threaded article processing
- Stop-word filtering for relevant search results

## Project Structure

```
.
├── rss-news-search.c       # Main search engine implementation
├── hashset.h               # Generic hash table interface
├── vector.h                # Generic dynamic array interface
├── html-utils.h            # HTML parsing utilities
├── url.h                   # URL handling utilities
├── urlconnection.h         # HTTP connection interface
├── streamtokenizer.h       # Text tokenization utilities
├── data/
│   ├── rss-feeds.txt       # RSS feed URLs
│   ├── rss-feeds-small.txt # Smaller feed list for testing
│   ├── stop-words.txt      # Common words to ignore
│   └── ...                 # Additional test feeds
└── Makefile                # Build configuration
```

## Features

- **RSS Feed Crawling**: Automatically fetches and parses RSS feeds from multiple news sources
- **Article Indexing**: Downloads full articles and extracts text content
- **Inverted Index**: Builds efficient search index mapping words to articles
- **Stop-Word Filtering**: Ignores common words (the, a, is, etc.) for better search results
- **Frequency Tracking**: Tracks word occurrence frequency in articles
- **Interactive Search**: Command-line interface for querying indexed articles

## Dependencies

The project requires libcurl for HTTP requests:

```sh
dpkg --add-architecture i386
apt update
apt install gcc make
apt install libcurl4-openssl-dev:i386
apt install libnsl-dev:i386
apt install gcc-multilib
```

## Building

```sh
make
```

The first build will automatically download the required data files.

### Rebuilding Data Files

If data files become corrupted:

```sh
rm -rf data/
make data
```

## Usage

Run the search engine:

```sh
./rss-news-search
```

The program will:

1. Load RSS feeds from the configured feed list
2. Download and parse articles
3. Build the inverted index
4. Present an interactive search prompt

Enter keywords to search, and the program will display matching articles with their URLs.

## Testing

**Run with memory checking:**

```sh
./assn-4-checker-64 ./rss-news-search
```

**Check for memory leaks:**

```sh
./assn-4-checker-64 ./rss-news-search -m
```

## Implementation Details

### String Hashing

The project uses a custom hash function for efficient string indexing:

```c
static const signed long kHashMultiplier = -1664117991L;
static int StringHash(const char *s, int numBuckets)
{
  unsigned long hashcode = 0;

  for (int i = 0; i < strlen(s); i++)
    hashcode = hashcode * kHashMultiplier + tolower(s[i]);

  return hashcode % numBuckets;
}
```

This hash function:

- Uses linear congruence method
- Is case-insensitive
- Provides good distribution across hash buckets

### Data Structures

The search engine uses:

- **HashSet**: For storing unique words and articles efficiently
- **Vector**: For storing article lists per word and word frequencies
- **Inverted Index**: Maps each word to a vector of articles containing that word

### Article Processing Pipeline

1. **Feed Parsing**: Extract article URLs from RSS XML
2. **Article Download**: Fetch full HTML content via HTTP
3. **Text Extraction**: Parse HTML and extract readable text
4. **Tokenization**: Split text into individual words
5. **Indexing**: Add words to inverted index with article references
6. **Stop-Word Filtering**: Remove common words that don't aid search

## RSS Feed Lists

Multiple feed configurations are included:

- `rss-feeds.txt`: Full list of news sources
- `rss-feeds-small.txt`: Smaller subset for faster testing
- `rss-feeds-techy.txt`: Technology-focused feeds
- `rss-feeds-tiny.txt`: Minimal set for debugging

## Learning Outcomes

This project demonstrates:

- Real-world application of data structures (hash tables, vectors)
- Network programming with HTTP
- XML and HTML parsing
- Text processing and tokenization
- Building a practical search engine from scratch
- Memory-efficient indexing of large text corpora
