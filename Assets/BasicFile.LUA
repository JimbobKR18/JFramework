print("Testing LUA script")
SLB.using(SLB)

-- Testing level loading
function LoadLevel(levelname)
	gameapp = StaticGameApp.GetApp()
	print("Got the app successfully")
	levelmanager = gameapp:GetLevelManager()
	print("Got the LevelManager successfully")
	gameapp2 = levelmanager:GetOwningApp()
	print("Got App from manager successfully")
	testlevel = levelmanager:CreateLevel("BasicLevel.txt")
	print("Got the level successfully")
	testlevel:Load()
	print("Loaded the level successfully")
end

-- Testing GameObject loading
function LoadObject(objectname)
	gameapp = StaticGameApp.GetApp()
	objectmanager = gameapp:GetObjectManager()
	print("Got the ObjectManager successfully")
	newobject = objectmanager:CreateObject("BasicObject.txt")
	print("Loaded the object successfully")
end
