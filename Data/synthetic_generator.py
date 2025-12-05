import random
import time

users = ["alpha", "beta", "gamma", "viewer", "guest", "pro", "noob"]
positive = ["nice", "gg", "awesome", "love it", "😂", "🔥"]
negative = ["bad", "trash", "hate", "lame", "🤮", "😡"]
spam = ["LOOOOOOOL", "FOLLOW FOR FOLLOW!!!", "BUY NOW!!!!", "AAAAAAAAAAAAA"]

def generate_message():
    msg_type = random.choice(["pos", "neg", "neutral", "spam"])
    if msg_type == "pos":
        return random.choice(positive)
    if msg_type == "neg":
        return random.choice(negative)
    if msg_type == "spam":
        return random.choice(spam)
    return "random chat message " + str(random.randint(0, 100))

with open("sample_chatlog.txt", "w", encoding="utf-8") as f:
    for _ in range(200000):   # number of messages
        timestamp = time.strftime("%H:%M:%S")
        user = random.choice(users)
        f.write(f"{timestamp} {user}: {generate_message()}\n")
