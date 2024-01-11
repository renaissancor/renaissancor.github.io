---
title: GraphQL Study II
date: 2023-09-24 21:15:20 +0900
categories: [Web, GraphQL]
tags: [graphql, api]		# tags MUST be small letters, like tag NOT TAG 
---

#### **Mutation** Introduction

```tsx
import { extendType, nonNull, objectType, stringArg } from "nexus";

export const LinkMutation = extendType({ // 1
    type: "Mutation",
    definition(t) {
        t.nonNull.field("post", { // 2
            type: "Link",
            args: { // 3
                description: nonNull(stringArg()),
                url: nonNull(stringArg()),
            },

            resolve(parent, args, context) {
                const { description, url } = args; // 4

                let idCount = links.length + 1; // 5
                const link = {
                    id: idCount,
                    description: description,
                    url: url,
                };
                links.push(link);
                return link;
            },
        });
    },
});
```

1. Extending the Mutation type by adding a new root field. Similar to what's done in queries.
2. Mutation name is "post," and it returns a non-nullable Link object.
3. Definition of mutation arguments, passing arguments to GraphQL API endpoints. Here, two arguments, description, and url, are defined as non-null.
4. The second argument, args, is used to access the provided arguments.
5. idCount is a basic way to generate a new id value for the link object. A new link object is created with the provided data, and it's pushed into the links array. The newly created object is returned.

Note: The `resolve` function should return an object and not null to avoid errors.

#### Database Schema Definition

```graphql
datasource db { // 1
  provider = "sqlite"
  url      = "file:./dev.db"
}

generator client { // 2
  provider = "prisma-client-js"
}

model Link { // 3 
  id          Int@id@default(autoincrement())   // 4
  createdAt   DateTime@default(now())  // 5
  description String
  url         String
}
```

1. Defining a data source for Prisma, specifying the usage of SQLite and providing the path to the SQLite file.
2. Configuring a generator to create the Prisma Client.
3. Defining a data model named Link, which represents the structure of the Link database table. Each field in the model includes the field name, type, and additional attributes (e.g., @id, @default, etc.).
4. The @id attribute indicates that the id field is the primary key of the Link table and uses auto-increment.
5. The @default attribute specifies that the createdAt field will have a default value of the current timestamp in ISO 8601 format (e.g., 2021-11-16T21:48:39.798Z).

#### Server & Database Connection to Client

```tsx
import { PrismaClient } from "@prisma/client";
export const prisma = new PrismaClient();

export interface Context { prisma: PrismaClient; }
export const context: Context = { prisma };
```

- The `Context` interface is defined to specify which objects are attached to the context object. In this case, it includes the PrismaClient.
- The context object is exported and can be imported and used by the GraphQL server.

Nexus Setup and Understanding the Type of GraphQL Context

- You can learn more about how to set up Nexus and understand the type of GraphQL context in your `src/schema.ts` file.

```tsx
return context.prisma.link.findMany(); // 1
const newLink = context.prisma.link.create({ // 2
```

1. Querying all Link records from the database using PrismaClient's `findMany` method.
2. Similar to the feed resolver, you use the PrismaClient instance. You also pass data via the `args` parameter to create a new link. Prisma Query methods return Promise objects, making both resolvers return Promises.

Handling TypeScript Prisma Errors

If you encounter TypeScript errors related to Prisma, follow these steps:

1. Restart your TypeScript server in VSCode: `CTRL + SHIFT + P`, then type: `restart TS Server`.

There may be type mismatches between the Nexus-generated type (number | undefined | null) and the expected Prisma type (number | undefined) for certain options. Type casting may be required because Nexus allows for nullable types, whereas Prisma does not consider null types.

Pagination Methods

Two common pagination methods:

1. **Limit-Offset**: Provide an index item and make special chunk requests.
2. **Cursor-Based**: Assign a unique ID to each element in the list. Clients paginate through the list using cursors, which are provided with each new element. This method allows for accurate item count tracking.