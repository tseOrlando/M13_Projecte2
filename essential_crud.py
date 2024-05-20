import pymongo
import random

class peer_t():
    
    def __init__(self, collection_name):
        self.peer       = pymongo.MongoClient("mongodb+srv://tese:1234@cluster0.0xtd9rf.mongodb.net/?retryWrites=true&w=majority")
        self.collection = self.peer["hard_motion"][collection_name]

#class to derive the collections and the general behaviors of each one

class essential_crud_t():

    def __init__(self, conn: peer_t):
        self.peer = conn.peer
        self.collection = conn.collection
        self.name       = self.collection.name
    
    def get_latest_id(self): 
        cursor = self.collection.find().limit(1).sort("$natural", -1)
        latest_document = next(cursor, None)

        if latest_document is None: return 0

        latest_id = latest_document["_id"]

        return int(latest_id)
    
    def get_all(self):       return list(self.collection.find())
    def get(self, id):       return self.collection.find_one({"_id" : id})
    def post(self, record):  return self.collection.insert_one(record).acknowledged
    def delete(self, id):    return self.collection.delete_one({"_id" : id}).acknowledged
    def put(self, id, data): return self.collection.update_one({"_id": id}, {"$set": data}).acknowledged