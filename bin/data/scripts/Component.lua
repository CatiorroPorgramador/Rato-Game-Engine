Engine.LoadDefaultLibraries()

GameObject = {
    Alive = true,
    Rect = {0, 0, 32, 32},
    filename = 'data/Skins/common-sheet.png',
    speed = 200,
    dx = 0,
    dy = 0,

    Init = function(self)

    end,

    Update = function(self, dt)
        -- Movement Keys
        if Engine.IsInputDown(Engine.KeyW) then
            dy = -self.speed * dt
        elseif Engine.IsInputDown(Engine.KeyS) then
            dy = self.speed * dt
        else
            dy = 0
        end
        
        if Engine.IsInputDown(Engine.KeyD) then
            dx = self.speed * dt
        elseif Engine.IsInputDown(Engine.KeyA) then
            dx = -self.speed * dt
        else
            dx = 0
        end

        if Engine.HasCollisionInGroup('Test') then
            self.Alive = false
            print('mora')
        end
        
        -- Apply
        self.Rect[1] = self.Rect[1] + dx
        self.Rect[2] = self.Rect[2] + dy
    end
}