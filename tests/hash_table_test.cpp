#include "tests.hpp"
#include <iostream>
#include <stdexcept>
#include <string>
#include "hash_table.hpp"

struct CharacterInfo
{
  size_t age;
  std::string animal;
};

std::ostream& operator<<(std::ostream& out, const CharacterInfo& info)
{
  out << info.animal << ", " << info.age << " years old";
  return out;
}

template< class TKey, class TValue >
void print(typename babinov::HashTable< TKey, TValue >::Iterator iter)
{
  std::cout << (*iter).first << " (" << (*iter).second << ')' << '\n';
}

template< class TKey, class TValue >
void print(typename babinov::HashTable< TKey, TValue >::ConstIterator iter)
{
  std::cout << (*iter).first << " (" << (*iter).second << ')' << '\n';
}

template< class TKey, class TValue >
void print(const babinov::HashTable< TKey, TValue >& table)
{
  for (auto it = table.cbegin(); it != table.cend(); ++it)
  {
    print< TKey, TValue >(it);
  }
  std::cout << "SIZE: " << table.size() << '\n';
  std::cout << "CAPACITY: " << table.bucketCount() << '\n';
}

void testHashTable()
{
  using namespace babinov;

  HashTable< std::string, CharacterInfo > characters;
  std::cout << characters.isEmpty() << '\n';

  HashTable< int, int > test({ {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5} });

  std::cout << "-------- INSERTION TEST: --------\n\n";

  auto res = characters.insert("Sonic", CharacterInfo{ 15, "Hedgehog" });
  std::cout << res.second << ' ' << (*res.first).first << ' ' << (*res.first).second << '\n';
  auto res2 = characters.insert("Tails", CharacterInfo{ 8, "Fox" });
  std::cout << res.second << ' ' << (*res2.first).first << ' ' << (*res2.first).second << '\n';
  auto res3 = characters.insert("Knuckles", CharacterInfo{ 16, "Echidna" });
  std::cout << res.second << ' ' << (*res3.first).first << ' ' << (*res3.first).second << '\n';
  std::cout << '\n';
  print(characters);
  std::cout << '\n';

  res = characters.insert("Sonic", CharacterInfo{ 15, "Hedgehog" });
  std::cout << res.second << ' ' << (*res.first).first << ' ' << (*res.first).second << '\n';
  res2 = characters.insert("Tails", CharacterInfo{ 8, "Fox" });
  std::cout << res.second << ' ' << (*res2.first).first << ' ' << (*res2.first).second << '\n';
  res3 = characters.insert("Knuckles", CharacterInfo{ 16, "Echidna" });
  std::cout << res.second << ' ' << (*res3.first).first << ' ' << (*res3.first).second << '\n';
  std::cout << '\n';
  print(characters);
  std::cout << '\n';

  characters["Amy Rose"] = CharacterInfo{ 12, "Hedgehog" };
  characters["Cream"] = CharacterInfo{ 6, "Rabbit" };
  characters["Silver"] = CharacterInfo{ 14, "Hedgehog" };
  print(characters);
  std::cout << '\n';

  characters["Amy Rose"] = CharacterInfo{ 12, "Hedgehog" };
  characters["Cream"] = CharacterInfo{ 6, "Rabbit" };
  characters["Silver"] = CharacterInfo{ 14, "Hedgehog" };
  print(characters);
  std::cout << '\n';

  characters["Shadow"] = CharacterInfo{ 65, "Hedgehog" };
  characters["Blaze"] = CharacterInfo{ 14, "Cat" };
  characters["Vector"] = CharacterInfo{ 20, "Crocodile" };
  characters["Espio"] = CharacterInfo{ 16, "Chameleon" };
  characters["Charmi"] = CharacterInfo{ 6, "Bee" };
  characters["Rouge"] = CharacterInfo{ 18, "Bat" };
  print(characters);
  std::cout << '\n';

  std::cout << "-------- REHASH TEST: --------\n\n";

  characters.rehash(0);
  print(characters);
  std::cout << '\n';

  characters.rehash(24);
  print(characters);
  std::cout << '\n';

  characters.rehash(10000);
  print(characters);
  std::cout << '\n';

  characters.rehash(0);
  print(characters);
  std::cout << '\n';

  std::cout << "-------- FIND ELEMENTS TEST: --------\n\n";

  for (auto it = characters.cbegin(); it != characters.cend(); ++it)
  {
    std::cout << (*characters.find((*it).first)).second << '\n';
  }
  std::cout << '\n';
  for (auto it = characters.cbegin(); it != characters.cend(); ++it)
  {
    std::cout << characters[(*it).first] << '\n';
  }
  std::cout << '\n';
  for (auto it = characters.cbegin(); it != characters.cend(); ++it)
  {
    std::cout << characters.at((*it).first) << '\n';
  }
  std::cout << '\n';

  std::cout << characters["Eggman"] << '\n';
  if (characters.find("EGGMAN") != characters.end())
  {
    std::cout << "Eggman is found" << '\n';
  }
  if (characters.find("Dark oak") == characters.end())
  {
    std::cout << "Dark oak is not found" << '\n';
  }

  try
  {
    characters.at("Dark oak");
  }
  catch (const std::out_of_range& e)
  {
    std::cout << e.what() << '\n';
  }
  std::cout << '\n';

  std::cout << "-------- COPYING AND MOVING TEST: --------\n\n";

  HashTable< std::string, CharacterInfo > charactersCopy(characters);
  HashTable< std::string, CharacterInfo > charactersCopy2;

  print(charactersCopy);
  std::cout << '\n';
  print(characters);
  std::cout << '\n';

  characters["Big"] = CharacterInfo{ 18, "Cat" };
  print(charactersCopy);
  std::cout << '\n';
  print(characters);
  std::cout << '\n';

  charactersCopy2 = characters;
  print(charactersCopy2);
  std::cout << '\n';
  print(characters);
  std::cout << '\n';

  characters["Whisper"] = CharacterInfo{ 16, "Wolf" };
  print(charactersCopy2);
  std::cout << '\n';
  print(characters);
  std::cout << '\n';

  HashTable< std::string, CharacterInfo > charactersCopy3(std::move(charactersCopy2));
  print(charactersCopy3);
  std::cout << '\n';

  charactersCopy2 = std::move(charactersCopy3);
  print(charactersCopy2);
  std::cout << '\n';

  std::cout << "-------- DELETING TEST: --------\n\n";

  charactersCopy2.erase("Charmi");
  charactersCopy2.erase("Eggman");
  charactersCopy2.erase("Cream");
  print(charactersCopy2);
  std::cout << '\n';

  auto it = charactersCopy2.begin();
  while (it != charactersCopy2.end())
  {
    auto temp = it;
    ++it;
    charactersCopy2.erase((*temp).first);
  }
  print(charactersCopy2);
  std::cout << '\n';

  charactersCopy3 = HashTable< std::string, CharacterInfo >(characters);
  auto it2 = charactersCopy3.begin();
  while (it2 != charactersCopy3.end())
  {
    it2 = charactersCopy3.erase(it2);
  }
  print(charactersCopy3);
  std::cout << '\n';

  charactersCopy3 = HashTable< std::string, CharacterInfo >(characters);
  charactersCopy3.clear();
  print(charactersCopy3);
  std::cout << '\n';
}
