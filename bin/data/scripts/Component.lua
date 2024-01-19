Engine.LoadDefaultLibraries()

GameComponent = {
    Alive = true,
    Rect = {0, 0, 64, 64},
    DirectionX = 0,
    DirectionY = 0,
    TextureID = Engine.LoadTextureID('data/Skins/monkey-sheet.png'),
    
    speed = 200.0,

    Init = function(self)
        Engine.EmitSignal("PlayerSpeed", 2)
        print(Engine.GetSignal("PlayerSpeed"))
    end,

    Update = function(self, dt)
        if Engine.HasCollisionInGroup('Test') then
            self.Alive = false
            print(Engine.GetSignal("PlayerSpeed"))
        end

        -- Movement Keys
        if Engine.IsInputDown(Engine.KeyW) then
            self.DirectionY = -self.speed * dt
        elseif Engine.IsInputDown(Engine.KeyS) then
            self.DirectionY = self.speed * dt
        else
            self.DirectionY = 0
        end
        
        if Engine.IsInputDown(Engine.KeyD) then
            self.DirectionX = self.speed * dt
        elseif Engine.IsInputDown(Engine.KeyA) then
            self.DirectionX = -self.speed * dt
        else
            self.DirectionX = 0
        end

        -- print('DirectionX: '..tostring(self.DirectionX))
    end
}