## Build, Lint, and Test

- **Build:** Use MSBuild to build the Visual Studio solution.
  - `msbuild WinAPI_Proj.vcxproj /p:Configuration=Debug`
  - `msbuild WinAPI_Proj.vcxproj /p:Configuration=Release`
- **Testing:** No automated testing framework is present. Manually test changes by running the application. Add a testing framework like Google Test if you need to add unit tests.

## Code Style

- **Naming Conventions:**
  - Classes: `PascalCase` with a 'C' prefix (e.g., `CPlayer`, `CEnemy`).
  - Functions: `PascalCase` (e.g., `CalculateDamage`, `Render`).
  - Variables: `camelCase` for local variables, `m_camelCase` for member variables.
  - Enums: `UPPER_SNAKE_CASE` (e.g., `PLAYER_STATE`, `ENEMY_TYPE`).
- **Formatting:**
  - Use tabs for indentation.
  - Place braces on the same line as the control statement (`if`, `for`, etc.).
- **Imports:**
  - Order imports from general to specific: system headers, library headers, then project headers.
- **Error Handling:**
  - Use `HRESULT` for functions that can fail and check the result. Avoid exceptions.
- **Memory Management:**
  - Use smart pointers (`std::unique_ptr`, `std::shared_ptr`) for resource management. Avoid raw `new` and `delete`.
