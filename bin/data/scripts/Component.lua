Engine.LoadDefaultLibraries()

GameObject = {
    Alive = true,
    Rect = {0, 0, 64, 64},
    -- SrcRect = {32, 0, 16, 16},
    TextureID = Engine.LoadTextureID('data/Skins/monkey-sheet.png'),
    
    speed = 200,
    dx = 0,
    dy = 0,

    Init = function(self)
        
    end,

    Update = function(self, dt)
        if Engine.HasCollisionInGroup('Test') then
            self.Alive = false
        end

        -- Movement Keys
        if Engine.IsInputDown(Engine.KeyW) then
            self.dy = -self.speed * dt
        elseif Engine.IsInputDown(Engine.KeyS) then
            self.dy = self.speed * dt
        else
            self.dy = 0
        end
        
        if Engine.IsInputDown(Engine.KeyD) then
            self.dx = self.speed * dt
        elseif Engine.IsInputDown(Engine.KeyA) then
            self.dx = -self.speed * dt
        else
            self.dx = 0
        end
        
        -- Apply
        self.Rect[1] = self.Rect[1] + self.dx
        self.Rect[2] = self.Rect[2] + self.dy
    end
}