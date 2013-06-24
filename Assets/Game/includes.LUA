SLB.using(SLB)

-- Testing level loading
function LoadLevel(levelname)
	gameapp = StaticGameApp.GetApp()
	print("Got the app successfully")
	levelmanager = gameapp:GetLevelManager()
	print("Got the LevelManager successfully")
	level = levelmanager:CreateLevel(levelname)
	print("Got the level successfully")
	level:Load()
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

