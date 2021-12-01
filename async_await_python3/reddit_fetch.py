import asyncio
import aiohttp
import sys
import json


loop = asyncio.get_event_loop()
client = aiohttp.ClientSession(loop=loop)


async def get_json(client: aiohttp.ClientSession, url):
    async with client.get(url) as response:
        assert response.status == 200
        return await response.read()


async def get_reddit_top(subreddit_name: str, clinet):
    data = await get_json(client, f'https://www.reddit.com/r/{subreddit_name}/top.json?sort=top&t=day&limit=5')

    json_data = json.loads(data.decode('utf-8'))
    for i in j['data']['children']:
        i_data = i['data']
        score = i_data['score']
        title = i_data['title']
        link = i_data['url']

        print(f"{score}: {title} ({link})")



def signal_handler(signal, frame):
    loop.stop()
    client.close()
    sys.exit(0)


def main():
    asyncio.ensure_future(get_reddit_top('python', client))
    asyncio.ensure_future(get_reddit_top('programming', client))


if __name__ == "__main__":
    main()
