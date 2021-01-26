#include "EntitySystem.h"

using namespace ::CainEngine::EntitySystem;

#include <random>

using CainEngine::EntitySystem::EntityID;

constexpr const int LoopCount = 100;

template<typename SceneType>
void InitScene(SceneType& scene, size_t entityCount, size_t hierarchyCount, vector<string>& row)
{
	constexpr const int Seed = 100;
	std::mt19937 rand{ Seed };
	std::uniform_int_distribution<size_t> randRootObject{ 0, entityCount - 1 };

	std::vector<EntityID> entities;

	const auto begin = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < entityCount; ++i)
	{
		entities.push_back(scene.Create(string("Entity") + std::to_string(i)));
	}

	std::vector<EntityID> hierarchyRoot;

	for (size_t i = 0; i < hierarchyCount; ++i)
	{
		auto entityIndex = randRootObject(rand);
		    
		while (scene.GetParent(entities[entityIndex]))
		{
			entityIndex = randRootObject(rand);
		}

		auto parentIndex = randRootObject(rand);

		scene.SetParent(entities[entityIndex], entities[parentIndex]);

		hierarchyRoot.push_back(entities[entityIndex]);
	}

	auto newHierarchyRoot = hierarchyRoot;
	hierarchyRoot.clear();

	size_t count = hierarchyCount / 2;
	while (count > 1)
	{
		const std::uniform_int_distribution<size_t> parentRand{ 0, (size_t)count - 1 };

		for (size_t i = 0; i < count; ++i)
		{
			auto entityIndex = randRootObject(rand);

			while (scene.GetParent(entities[entityIndex]))
			{
				entityIndex = randRootObject(rand);
			}

			const auto parentIndex = parentRand(rand);

			scene.SetParent(entities[entityIndex], entities[parentIndex]);

			hierarchyRoot.push_back(entities[entityIndex]);
		}

		newHierarchyRoot = hierarchyRoot;
		hierarchyRoot.clear();
		count /= 4;
	}

	const auto end = std::chrono::high_resolution_clock::now();

	const std::chrono::duration<double, std::milli> delta = (end - begin);

	printf("- %s - Init: %.2f ms\n", mst::typename_of<SceneType>(), delta.count());

	row.push_back(mst::to_printf_string("%.2f", delta.count()));

	scene.BuildMatrices();
}

template<typename SceneType>
void RemoveEntities(SceneType& scene, size_t entityCount)
{
	constexpr const int Seed = 100;
	std::mt19937 rand{ Seed };

	vector<EntityID> entities;
	entities.reserve(scene.LiveEntities());

	scene.ForEach([&entities](EntityID id)
	{
		entities.push_back(id);
	});

	if (entityCount >= entities.size())
	{
		for(auto e : entities)
		{
			scene.Destroy(e);
		}
	}
	else
	{
		std::uniform_int_distribution<size_t> randRootObject{ 0, entities.size() - 1 };

		for (size_t i = 0; i < entityCount; ++i)
		{
			auto entity = entities[randRootObject(rand)];

			while (!scene.IsAlive(entity))
			{
				entity = entities[randRootObject(rand)];
			}

			scene.Destroy(entity);

			//COMMON_ASSERT(scene.DeadEntities() == scene.Checksum());
		}

	}
}

template<typename SceneType>
void Matrices(SceneType& scene, size_t pollCount, size_t randomMutationCount, vector<string>& row)
{
	constexpr const int Seed = 100;
	std::mt19937 rand{ Seed };

	vector<EntityID> entities;
	entities.reserve(scene.LiveEntities());

	scene.ForEach([&entities](EntityID id)
	{
		entities.push_back(id);
	});

	std::uniform_int_distribution<size_t> randEntity{ 0, entities.size() - 1 };

	const auto begin = std::chrono::high_resolution_clock::now();

	vector<matrix4x3> matrices;
	matrices.reserve(pollCount);

	for (int i = 0; i < LoopCount; ++i)
	{
		matrices.clear();

		for (size_t j = 0; j < randomMutationCount; ++j)
		{
			auto entityIndex = randEntity(rand);

			scene.GetTransform(entities[entityIndex]).Translate({ 0, 0, 1 });
		}

		scene.BuildMatrices();

		auto frameIndex = (uint32_t)i + 1;
		for (size_t j = 0; j < pollCount; ++j)
		{
			matrices.push_back(scene.GetGlobalMatrix(entities[randEntity(rand)], frameIndex));
		}
	}

	const auto end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double, std::milli> delta = (end - begin) / LoopCount;

	printf("- %s - Matrices: %.2f ms\n", mst::typename_of<SceneType>(), delta.count());
	row.push_back(mst::to_printf_string("%.2f", delta.count()));
}

template<typename SceneType>
void Iteration(SceneType& scene, vector<string>& row)
{
	const auto begin = std::chrono::high_resolution_clock::now();

	uint32_t val = 0;

	scene.ForEach([&val](EntityID id)
	{
		val += id.Value();
	});

	const auto end = std::chrono::high_resolution_clock::now();

	const std::chrono::duration<double, std::milli> delta = (end - begin);

	printf("- %s - Iteration: %.2f ms, checksum: %u\n", mst::typename_of<SceneType>(), delta.count(), val);
	row.push_back(mst::to_printf_string("%.2f", delta.count()));
}

template<typename SceneType>
void Benchmark(size_t entityCount, size_t hierarchyCount, size_t pollCount, std::ofstream& csv)
{
	vector<string> row;
	row.reserve(16);

	SceneType scene;

	row.push_back(mst::typename_of<SceneType>());
	row.push_back(std::to_string(entityCount) + '&' + std::to_string(hierarchyCount));

	InitScene(scene, entityCount, hierarchyCount, row);

	/*Matrices(scene, pollCount, 0, row);
	Matrices(scene, pollCount, pollCount, row);
	Matrices(scene, pollCount, pollCount / 2, row);
	Matrices(scene, pollCount, pollCount / 3, row);
	Matrices(scene, pollCount, pollCount / 4, row);*/

	Iteration(scene, row);

	RemoveEntities(scene, entityCount / 4);

	Iteration(scene, row);

	RemoveEntities(scene, entityCount / 4);

	Iteration(scene, row);

	RemoveEntities(scene, entityCount);

	Iteration(scene, row);

	scene.Create();
	scene.Clear();

	Iteration(scene, row);

	for (size_t i = 0; i < row.size() - 1; ++i)
	{
		csv << ('"' + row[i] + "\";");
	}

	csv << ('"' + row.back() + "\"\n");
}

void RunBenchmark()
{
	tuple<size_t, size_t, size_t> sceneConfigs[] = { {100000, 3000, 90000 }, {100000, 30000, 90000} };

	std::ofstream csv (platform::downloads_path() + platform::directory_separator() + "results.csv");

	csv << "\"Scene type\";\"Scene config\";\"Init scene\";\"Static scene\";\"Dynamic scene\";\"Half dynamic scene\";\"Third dynamic scene\";\"Quarter dynamic scene\";\"Iteration on 100% alive\";\"Iteration on 75% alive\";\"Iteration on 50% alive\";\"Iteration on 0% alive\";\"Iteration on cleared scene\"\n";

	for (auto& config : sceneConfigs)
	{
		printf("Config: %llu, %llu, %llu\n", (uint64_t)std::get<0>(config), (uint64_t)std::get<1>(config), std::get<2>(config));

		printf("InitScene scene:\n");

		printf("Static scene:\n");

		Benchmark<CainEngine::EntitySystem::Scene>(std::get<0>(config), std::get<1>(config), std::get<2>(config), csv);
		Benchmark<CainEngine::EntitySystem::Scene2>(std::get<0>(config), std::get<1>(config), std::get<2>(config), csv);
		Benchmark<CainEngine::EntitySystem::Scene3>(std::get<0>(config), std::get<1>(config), std::get<2>(config), csv);

		printf("\n");
	}
}

void RunTest()
{
	Scene3 scene;

	vector<EntityID> entities;
	for (int i = 0; i < 10; ++i)
	{
		entities.push_back(scene.Create());
	}

	scene.Destroy(entities[4]);
	scene.Destroy(entities[3]);
	scene.Destroy(entities[0]);
	scene.Destroy(entities[1]);
	scene.Destroy(entities[2]);

	scene.Destroy(entities[9]);

	vector<EntityID> newEntities;

	for (int i = 0; i < 6; ++i)
	{
		newEntities.push_back(scene.Create());
	}

	// Non recycled
	newEntities.push_back(scene.Create());
}

void Print()
{
	Scene3 scene;

	vector<EntityID> entities;
	for (int i = 0; i < 10; ++i)
	{
		entities.push_back(scene.Create());
	}

	scene.Destroy(entities[4]);
	scene.Destroy(entities[3]);
	scene.Destroy(entities[0]);
	scene.Destroy(entities[1]);
	scene.Destroy(entities[2]);

	scene.Destroy(entities[9]);

	vector<EntityID> newEntities;

	for (int i = 0; i < 6; ++i)
	{
		newEntities.push_back(scene.Create());
	}

	// Non recycled
	for (int i = 0; i < 6; ++i)
	{
		newEntities.push_back(scene.Create());
	}

	std::mt19937 engine;
	std::uniform_int_distribution<size_t> rand(0, scene.LiveEntities() - 1);
	for (int i = 0; i < 10; ++i)
	{
		newEntities.push_back(scene.Create());
	}
}

int main()
{
	Print();

	RunTest();

	RunBenchmark();

	std::cout << "Press enter to exit...";
	std::string line;
	std::getline(std::cin, line);
}