#include "gui/Views/ProjectView.h"
// #include "gui/Views/SettingsView.h"
#include "gui/Manager.h"
#include "PA2528-2/PoolAllocator.h"
#include "PA2528-3/ResourceManager.hpp"
#include "PA2528-2/MemoryTracker.h"

namespace gui
{
	void ProjectView::QuitGame(int action)
	{
		if (action == GLFW_RELEASE)
		{
			QuitGameEvent event;
			_eventManager->execute(event);
		}
	}
	ProjectView::ProjectView(EventManager* manager, float* fps) : _eventManager(manager), _fps(fps), View()
	{
		_name = "ProjectView";

		gui::Font *font = Factory::getInstance().getFont("Resources/fonts/arial");
		gui::Label *l = new Label(font);
		l->addStringComponent(new StringComponentString("GAMENAME"));
		l->setPosition(0.0f - l->getSize().x * 0.5f, 0.8f - l->getSize().y / 2.0f);
		addChild(l);

		_quitButton = new gui::Button(0.75f, 0.3f);
		_quitButton->addStringComponent(new StringComponentString("Quit"));
		_quitButton->setPosition(-_quitButton->getSize().x*0.25f + 0.75f, -0.9f);
		_quitButton->setPrimaryColor(PALLETPRIMARY);
		_quitButton->setSecondaryColor(PALLETHIGHLIGHT);
		_quitButton->listen(this, &ProjectView::QuitGame);
		_quitButton->setScale(0.5, 0.5);
		addChild(_quitButton);

		memoryLimitBar = new gui::ProgressBar(0.1f, 0.029f);
		memoryLimitBar->setSecondaryColor(glm::vec4(0.9f, 0.9f, 0.9f, 1.0f));
		memoryLimitBar->setPrimaryColor(glm::vec4(0.4f, 0.4f, 0.4f, 1.0f));
		memoryLimitBar->setPosition(glm::vec2(-0.965f, 0.92f));
		memoryLimitBar->setValue(ResourceManager::getMemoryFillRatio());
		addChild(memoryLimitBar);

		memoryLimitLabel = new gui::Label(font);
		memoryLimitLabel->addStringComponent(new StringComponentString("Memory Limit"));
		memoryLimitLabel->setScale(0.15f, 0.25f);
		memoryLimitLabel->setPosition(glm::vec2(-0.845f, 0.92f));
		addChild(memoryLimitLabel);

		cacheHitsLabel = new gui::Label(font);
		cacheHitsLabel->addStringComponent(new StringComponentString("Cache Hits: "));
		cacheHitsLabel->addStringComponent(new StringComponentInt(&cacheHits));
		cacheHitsLabel->setScale(0.15f, 0.25f);
		cacheHitsLabel->setPosition(glm::vec2(-0.965f, 0.87f));
		addChild(cacheHitsLabel);

		cacheMissesLabel = new gui::Label(font);
		cacheMissesLabel->addStringComponent(new StringComponentString("Cache Misses: "));
		cacheMissesLabel->addStringComponent(new StringComponentInt(&cacheMisses));
		cacheMissesLabel->setScale(0.15f, 0.25f);
		cacheMissesLabel->setPosition(glm::vec2(-0.965f, 0.82f));
		addChild(cacheMissesLabel);

		VRAMLabel = new gui::Label(font);
		VRAMLabel->addStringComponent(new StringComponentString("VRAM: "));
		VRAMLabel->addStringComponent(new StringComponentInt(&currentVRAM));
		VRAMLabel->setScale(0.15f, 0.25f);
		VRAMLabel->setPosition(glm::vec2(-0.965f, 0.77f));
		addChild(VRAMLabel);

		l = new gui::Label(font);
		l->addStringComponent(new StringComponentString("FPS: "));
		l->addStringComponent(new StringComponentFloat(_fps));
		l->setPosition(1.0f - l->getSize().x * 0.1, 0.95f - l->getSize().y *0.25);
		l->setScale(0.15f, 0.25f);
		addChild(l);
	}
	ProjectView::~ProjectView()
	{
	}
	void ProjectView::onRender(float dt)
	{
		for (int i = 0; i < memoryUsageBars.size(); ++i) {
			memoryUsageBars[i]->render(dt);
		}
	}
	void ProjectView::onUpdate(float dt)
	{
		PoolAllocator pa(4, 4);
		pa.setTrackingName("PoolAllocator 1");
		int* asdf = pa.alloc<int>(5);

		PoolAllocator pa2(4, 4, 16);
		pa2.setTrackingName("PoolAllocator 2");
		int* qwer = pa2.alloc<int>(5);

		// Update memory usage progress bars
		std::vector<AllocatorInfo> infos = MemoryTracker::getAllocatorsInfo();

		// Sort to avoid flickering
		std::sort(infos.begin(), infos.end(),
			[](const AllocatorInfo& a, const AllocatorInfo& b) -> bool
			{
				return a.allocatorID > b.allocatorID;
			}
		);
		if (infos.size() > memoryUsageBars.size()) {
			int diff = infos.size() - memoryUsageBars.size();
			for (int i = 0; i < diff; ++i) {
				gui::ProgressBar* newBar = new gui::ProgressBar(0.1f, 0.029f);
				newBar->setSecondaryColor(glm::vec4(0.9f, 0.9f, 0.9f, 1.0f));
				newBar->setPrimaryColor(glm::vec4(0.4f, 0.4f, 0.4f, 1.0f));
				memoryUsageBars.push_back(newBar);
				addChild(newBar);

				gui::Font *font = Factory::getInstance().getFont("Resources/fonts/arial");
				gui::Label* newLabel = new gui::Label(font);
				newLabel->addStringComponent(new StringComponentString(infos[i].allocatorName));
				memoryUsageLabels.push_back(newLabel);
				addChild(newLabel);
			}
		}

		for (int i = 0; i < memoryUsageBars.size(); ++i) {
			if (i >= infos.size()) {
				memoryUsageBars[i]->setScale(0.0f);
				memoryUsageLabels[i]->setScale(0.0f);
			}
			else {
				memoryUsageBars[i]->setScale(1.0f);
				memoryUsageBars[i]->setPosition(glm::vec2(-0.965f, -0.95f + 0.05f * i));
				memoryUsageBars[i]->setValue(((float)infos[i].totalUsed) / ((float)infos[i].totalReserved));

				memoryUsageLabels[i]->setScale(0.15f, 0.25f);
				memoryUsageLabels[i]->setPosition(glm::vec2(-0.845f, -0.95f + 0.05f * i));
				memoryUsageLabels[i]->clearStringComponents();
				memoryUsageLabels[i]->addStringComponent(new StringComponentString(infos[i].allocatorName));
			}
		}
		pa.dealloc<int>(asdf);
		pa2.dealloc<int>(qwer);

		// Update memory limit bar
		memoryLimitBar->setValue(ResourceManager::getMemoryFillRatio());

		// Update chache miss/hits labels
		cacheHits = (int)MemoryTracker::getResourceManagerCacheHits();
		cacheMisses = (int)MemoryTracker::getResourceManagerCacheMisses();

		// Update VRAM label
		currentVRAM = (int)MemoryTracker::getVRAM();
	}
	void ProjectView::initiate()
	{
		cursorModeChangeEvent cEvent(GLFW_CURSOR_NORMAL);
		_eventManager->execute(cEvent);
	}
}
