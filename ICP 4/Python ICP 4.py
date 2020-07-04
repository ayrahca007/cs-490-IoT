from twython import Twython

apiKey = 'Udop9tfuG1pLl2yjeWTeIRVNy'
apiSecret = 'xdhgC1PVLHZN70ZFKaVvoA7Zz0YLxn2ks481btSJZrTZUjkgDg'
accessToken = '239771981-NvxUyC5qrYj5g4vlQyoenCgbhWaYaR8zCAHgsw3p'
accessTokenSecret = 'DdLF1sCY1i7wtNafJzN2kzxDpBLXHDwDzUc3fiplwzv1c'
twitter = Twython(apiKey, apiSecret, accessToken, accessTokenSecret)

photo = open('/home/pi/Desktop/Image.jpg', 'rb')
text = "Coming to you live from Python!"

response = twitter.upload_media(media=photo)
twitter.update_status(status=text, media_ids=[response['media_id']])
