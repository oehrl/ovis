Renderer
{
	name = 'PathEditingRenderer'
}

PathEditingController = Controller();

function PathEditingController:OnUpdate(delta_time)
end

function PathEditingController:OnEvent(event)
	if event.type == 'KeyDown'
		if event.key == 'A'
			print('A');
		end
	end
end