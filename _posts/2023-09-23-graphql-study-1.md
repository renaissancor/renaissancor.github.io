---
title: GraphQL Study I
date: 2023-09-23 21:15:20 +0900
categories: [Web, GraphQL]
tags: [graphql, api]		# tags MUST be small letters, like tag NOT TAG 
---

Relay GraphQL problem: Relay source code has compiler to continuously optimize graphql`content` code by parsing strping template. 
Its compiler uses watchman. No tutorial info about it in relay tutorial 

1. GraphQL Schema has multiple types. Currently empty.
2. The first output is the schema.graphql file, defining the GraphQL SDL (Schema Def. Language) that structures the API.
3. The second output is the TS typegen file containing definitions of TypeScript types for graphql.schema, ensuring type safety and maintaining schema consistency when changes occur.

To update these two files, run `npm run generate`, and to start the server, run `npm run dev`.

```json
"dev": "ts-node-dev --transpile-only --no-notify --exit-child src/index.ts",
"generate": "ts-node --transpile-only src/schema.ts”
```

**Query** Introduction:

- Querying the `ok` field: 
  ```graphql
  query Query {
    ok
  }
  ```
  Response: 
  ```json
  {
    "data": {
      "ok": true
    }
  }
  ```
  This is an example of a GraphQL Query.

- Query for all users:
  ```graphql
  query {
    users {
      id
      name
    }
  }
  ```

- Query a single user by their ID:
  ```graphql
  query {
    user(id: "user-1") {
      id
      name
    }
  }
  ```

- Create a new user:
  ```graphql
  mutation {
    createUser(name: "Bob") {
      id
      name
    }
  }
  ```

Code-First Approach to GraphQL API Development:

1. Define types, fields, and root object types within the schema.
2. Generate GraphQL SDL and types.
3. Add appropriate resolver functions.

#### Defining a GraphQL Object Type

```jsx
import { objectType } from "nexus";

// 1. Type name
export const Link = objectType({
    name: "Link",

    // 2. Definition: Adding type elements
    definition(t) {
        // 3. Adding a non-null integer field "id"
        t.nonNull.int("id");
        // 4. Adding a non-null string field "description"
        t.nonNull.string("description");
        // 5. Adding a non-null string field "url"
        t.nonNull.string("url");
    },
});
```

1. Name of the GraphQL object type.
2. Definition block where type elements are added.
3. Adding a non-null integer field "id."
4. Adding a non-null string field "description."
5. Adding a non-null string field "url."

#### Integrating the GraphQL Object Type into Schema

```jsx
import * as types from "./graphql";   // 1

export const schema = makeSchema({
    types,   // 2
    outputs: {
        typegen: join(process.cwd(), "nexus-typegen.ts"),
        schema: join(process.cwd(), "schema.graphql"),
    },
});
```

1. Importing the GraphQL model that exports the Link object type.
2. Sending the imported types to the `makeSchema` function, which Nexus uses to generate the SDL files.

#### Resulting schema.graphql

```graphql
type Link {
  description: String!
  id: Int!
  url: String!
}
```

The `schema.graphql` file contains the definition for the `Link` object type.

#### nexus-typegen.ts

In the `nexus-typegen.ts` file, you will find an interface for the `Link` object type, ensuring consistency with the schema definition.
