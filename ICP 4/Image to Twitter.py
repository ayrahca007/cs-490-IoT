# Uploads an image from your pc to Twitter
from twython import Twython

# Twitter API Information; From Project Details on Twitter Developers
apiKey = 'xxxxxxxxxxxx'
apiSecret = 'xxxxxxxxxxxx'
accessToken = 'xxxxxxxxxxxx'
accessTokenSecret = 'xxxxxxxxxxxx'
twitter = Twython(apiKey, apiSecret, accessToken, accessTokenSecret)

photo = open('/home/pi/Desktop/Image.jpg', 'rb') #Image Path
text = "Coming to you live from Python!"        #Image Caption

response = twitter.upload_media(media=photo)
twitter.update_status(status=text, media_ids=[response['media_id']])
