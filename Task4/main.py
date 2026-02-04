import requests
import json
import os
from bs4 import BeautifulSoup
from collections import deque
from urllib.parse import urljoin
from concurrent.futures import ThreadPoolExecutor
import threading

BASE_URL = "https://en.wikipedia.org"
TARGET_PATH = "/wiki/Adolf_Hitler"
CACHE_FILE = "wiki_cache.json"
MAX_WORKERS = 10


class CacheManager:
    def __init__(self, filename):
        self.filename = filename
        self.lock = threading.Lock()
        self.cache = self._load_cache()

    def _load_cache(self):
        if os.path.exists(self.filename):
            try:
                with open(self.filename, 'r', encoding='utf-8') as f:
                    return json.load(f)
            except:
                return {}
        return {}

    def get_links(self, path):
        with self.lock:
            return self.cache.get(path)

    def save_links(self, path, links):
        with self.lock:
            self.cache[path] = links
            with open(self.filename, 'w', encoding='utf-8') as f:
                json.dump(self.cache, f, ensure_ascii=False, indent=4)


cache_manager = CacheManager(CACHE_FILE)
visited_lock = threading.Lock()


def get_links(path):
    cached_data = cache_manager.get_links(path)
    if cached_data is not None:
        return cached_data

    url = urljoin(BASE_URL, path)
    try:
        headers = {
            'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36',
            'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8',
            'Accept-Language': 'en-US,en;q=0.5',
            'Referer': 'https://www.google.com/'
        }

        response = requests.get(url, timeout=10, headers=headers)

        if response.status_code != 200:
            return []

        soup = BeautifulSoup(response.text, 'html.parser')
        links = []
        content = soup.find(id="bodyContent")

        if content:
            for link in content.find_all('a', href=True):
                href = link['href']
                if href.startswith('/wiki/') and ':' not in href and '#' not in href:
                    links.append(href)

        unique_links = list(set(links))
        cache_manager.save_links(path, unique_links)
        print(f"Scanning: {path} | Found: {len(unique_links)}")
        return unique_links
    except Exception as e:
        print(f"Exception on {path}: {e}")
        return []


def find_hitler(start_url):
    start_path = "/wiki/" + start_url.split("/")[-1] if "http" in start_url else start_url
    if not start_path.startswith("/wiki/"):
        start_path = "/wiki/" + start_url.split("/")[-1]

    queue = deque([[start_path]])
    visited = {start_path}

    for hop in range(6):
        print(f"\n--- Level {hop + 1} | Queue: {len(queue)} ---")

        current_level_paths = []
        while queue:
            current_level_paths.append(queue.popleft())

        with ThreadPoolExecutor(max_workers=MAX_WORKERS) as executor:
            results = list(executor.map(lambda p: get_links(p[-1]), current_level_paths))
        for i, links in enumerate(results):
            for link in links:
                if link == TARGET_PATH:
                    return current_level_paths[i] + [link]

                with visited_lock:
                    if link not in visited:
                        visited.add(link)
                        queue.append(current_level_paths[i] + [link])

        if not queue:
            break

    return None


def main():
    url = input("Enter Wikipedia link: ").strip()
    if not url: return

    print("\nSearching...")
    result = find_hitler(url)

    if result:
        print("\nPath found!")
        print(" -> ".join(result))
    else:
        print("\nHitler not found within 6 hops.")


if __name__ == "__main__":
    main()