from fastapi import fastapi, Request, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from bson import ObjectId
import datetime
import motor.motor_asyncio
import os
import pydantic



app = fastapi()
client = motor.motor_asyncio.AsyncIOMotorClient(os.getenv("MONGO_DB"))
database = client.profile



pydantic.json.ENCODERS_BY_TYPE[ObjectId] = str

origins = [    
    "http://localhost:8000"
]

app.add_middleware(
    CORSMiddleware,
    allow_origins=origins,
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)


print(os.getenv("MONGO_DB"))


@app.get("/api/state")
async def get_values(request : Request):
    temperature = await client["temp"].find().sort("date").to_list(1)
    
    fan_on = False

    if temperature < 28.0:
        fan_on = True
    else:
        fan_on = False

    current_time = datetime.datetime.now
    response = request.get("https://ecse-sunset-api.onrender.com/api/state")
    response_json = response.json()
    sunset_time = response_json["sunset"]
    sunset_datetime = datetime.strptime(sunset_time, "%Y-%m-%dT%H:%M:%S.%f")
    
    if sunset_datetime > current_time: 
        light_on = False
    else: 
         light_on = True

    return {
        "is light on":light_on,
        "is fan on":fan_on
    }

@app.put("/api/temperature", status_code = 204)
async def set_values(request : Request):
    this_object = await request.json()
    this_object["date"] = datetime.datetime.now()
    inserted_object = await client["temp"].insert_one(this_object)

    check = await client["temp"].find_one({"_id": inserted_object.inserted_id})

    if check == None: 
        raise HTTPException(status_code=400, detail = "unsuccessful")

