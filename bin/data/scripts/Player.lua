Engine.LoadDefaultLibs()

Player = {
    Name = 'Player',
    SrcRect = {0, 0, 16, 16},
    DstRect = {0, 0, 16*4, 16*4},

    Update = function()
        if Engine.IsInputDown(Engine.KeyA) then
            print("Apertou A")
        end
    end,
}