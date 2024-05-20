from fastapi import FastAPI
from essential_crud import essential_crud_t, peer_t

# set up the whole api here in a clean and easy way

api = FastAPI()
_event_api = essential_crud_t(peer_t("events"))
_member_api = essential_crud_t(peer_t("members"))

def helper_verify_id_range(id : str, c : essential_crud_t): return (int(id) > 0 and int(id) <= c.get_latest_id())  

#event api
@api.get("/events")
def get_all_events(): 
    return _event_api.get_all()

@api.get("/event/{event_id}")
def get_event(event_id):    
    return _event_api.get(event_id)

@api.get("/event_members/{event_id}")
def get_event_members(event_id):
    if helper_verify_id_range(event_id, _event_api):   
        result = _event_api.collection.find_one({"_id": event_id}, {"_id": 0, "members": 1})
        if result is not None:
            final_result = list(_member_api.collection.find({"_id": {"$in": result["members"]}}))
            return {"members":final_result} if final_result else {"error":"this event doesn't have members."}
    
    return {"error": "event was not found"}

@api.post("/event")
async def post_event(record : dict):    
    return _event_api.post(record)

@api.delete("/event/{event_id}")
async def delete_event(event_id: str):  
    return {_member_api.collection.update_many({}, {"$pull":{"events": event_id}}).acknowledged:_event_api.collection.delete_one({"_id": event_id}).acknowledged}

@api.get("/events/latest")
def get_latest_event_id(): 
    return _event_api.get_latest_id()

#member api
@api.get("/members")
def get_all_members(): 
    return _member_api.get_all()

@api.get("/member/{member_id}")
def get_member(member_id):    
    return _member_api.get(member_id)

@api.get("/member_name/{member_name}")
def get_member_by_name_api(member_name):    
    result = _member_api.collection.find_one({"name": member_name})

    return result if result else {"error":"user does not exist."}

@api.get("/member_events/{member_id}")
def get_member_events(member_id):   
    if helper_verify_id_range(member_id, _member_api):  
        result = _member_api.collection.find_one({"_id": member_id}, {"_id": 0, "events": 1})
        final_result = list(_event_api.collection.find({"_id": {"$in": result["events"]}}))
        return {"events":final_result} if final_result else {"error":"this member doesn't have events joined."}
    
    return {"error": "member was not found"}

@api.post("/member")
async def post_member(record : dict):
    if get_member_by_name_api(record["name"])["error"]: 
        return _member_api.post(record)
    
    return False

@api.delete("/member/{member_id}")
def delete_member(member_id): 
    return {_event_api.collection.update_many({}, {"$pull":{"members": member_id}}).acknowledged:_member_api.collection.delete_one({"_id": member_id}).acknowledged}

@api.delete("/member_deletion_by_name/{member_name}")
def delete_member_by_name(member_name):
    member_id  = get_member_by_name_api(member_name)["_id"] 
    return delete_member(member_id)

@api.get("/members/latest")
def get_latest_member_id(): 
    return _member_api.get_latest_id()

@api.put("/member/{member_id}/join/{event_id}")
def member_join_event(member_id, event_id): 
    result_1 = _member_api.collection.update_one({"_id": member_id}, {"$addToSet": {"events": event_id}}).acknowledged
    result_2 = _event_api.collection.update_one({"_id": event_id}, {"$addToSet": {"members": member_id}}).acknowledged

    return {result_1 : result_2}

@api.put("/update_member/{member_id}")
def update_member(member_id, data : dict): 
    return _member_api.put(member_id, data)